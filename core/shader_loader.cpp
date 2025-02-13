#include "shader_loader.h"

// os/platform
#include "platform.h"
#include "basic_types.h" // os or core?
#include "file_stat.h"   // should be os

// libs
//#include "OpenGL/gl3w.h"
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

// core
#include "cpu_timer.h"
#include "string_range.h"

// maths
#include "math/maths.h" // for min

// std
#include <sstream>
#include <vector>

#pragma warning(disable:4996) // fopen is unsafe

// Meta note: Are any of these notes still true?

// notes:
// * programs never remove themselves from users's lists of shaders. they could (the indecies are what they need to remove) but in practice a program is defined literally by the list of shaders it uses
//   so if a program is no longer in use, we can reclaim it, and then remove it from it's shaders' lists. but in practice it's not clear how to tell when a program is no longer in use (since it might be temp or not)
//   easiest to just let it ride, and it will get removed on next reboot. the same goes for "file not founds"

// todo
// * when stat returns access denied, don't flash filenames, just let them stay light or dark
// * flash filenames when they do update
// * can i get shader types from handles instead of storing them? maybe not since you may need to know the type before it compiles correctly?
// * replace all "constant" string messages with enums and prebaked strings (or just infer them at the gui callsite)


static void guiSetErroredFile(int idx);
static void guiSetErroredProg(int idx);

namespace { 

	struct Include {
		const char* start = 0;     // start of the include replacement in this file
		const char* end = 0;       // end of the replacement
		int line = 0;
		int file_idx = -1; // the file to replace it with (recursively)
	};

	struct FileEntry {
		char* pathfile_buff = NULL;
		StringRange pathfile;
		StringRange path;
		StringRange name;
		StringRange file;
		time_t last_modified = 0;
		bool not_found = false;         // needed because last_modified doesn't detect files coming into existence
		char* text = 0;                 // text of the last found file (can be stale, if file no longer exists, or is denied access)
		size_t text_len = 0;
		bool is_procedural = false;     // file is procedurally generated, instead of read in from disk
		char* procedural_text = 0;      // the procedurally generated text and length (as if it was read from disk)
		size_t procedural_text_len = 0;
		bool procedurally_modified = false;
		std::vector<int> user_idxs;     // file indecies that are using (including) this file
										//std::vector<int> include_idxs;  // file idxs that this file includes
		std::vector<Include> includes;
		int shader_idx = -1;			// shader to notify on any change
		void addUser(int idx);
		void remUser(int idx);
	};

	struct ShaderEntry {
		int file_idx = -1;           // the main text file of this shader
		int handle = 0;              // compiled shader. 0 if anything went wrong
		GLenum type = 0;             // the type of shader
		bool stale = true;           // set true by files if this shader's file or any that it includes has changed. set true by an attempted recompile (successful or not)
		std::vector<int> prog_idxs;  // the indecies of programs to notify of changes
		String log;
		String final_text;
		s64 time_to_compile = 0;
		char** source_dump_location = 0;
	};

	struct ProgEntry {
		int handle = 0;
		int vert_idx = -1;
		int frag_idx = -1;
		int comp_idx = -1;
		bool try_relink = true;
		bool stale = true;
		bool valid = false;
		bool rebuild_occurred = false;
		String name;
		String log;
		s64 time_to_link = 0;
	};

}

static std::vector<FileEntry> file_entries;
static std::vector<ShaderEntry> shader_entries;
static std::vector<ProgEntry> prog_entries;
static bool any_errors_since_last_check = false;

static void checkForUpdates(int file_idx);

static int getFileEntry(StringRange pathfile, bool is_main) {
	ASSERT(pathfile); // used below, during path extraction
	for (int i = 0; i < (int)file_entries.size(); ++i) {
		if (file_entries[i].pathfile == pathfile) {
			return i;
		}
	}
	// create a new entry
	int idx = (int)file_entries.size();
	int shader_idx = -1;
	if (is_main) {
		ShaderEntry s;
		s.file_idx = idx;
		const char* ext_str = pathfile.str + pathfile.len - min(size_t(4), pathfile.len);
		if (strcmp(ext_str, "vert") == 0) s.type = GL_VERTEX_SHADER;
		else if (strcmp(ext_str, "frag") == 0) s.type = GL_FRAGMENT_SHADER;
		else if (strcmp(ext_str, "comp") == 0) s.type = GL_COMPUTE_SHADER;
		else if (strcmp(ext_str, "geom") == 0) s.type = GL_GEOMETRY_SHADER;
		else if (strcmp(ext_str, "ctrl") == 0) s.type = GL_TESS_CONTROL_SHADER;
		else if (strcmp(ext_str, "eval") == 0) s.type = GL_TESS_EVALUATION_SHADER;
		shader_idx = (int)shader_entries.size();
		shader_entries.push_back(s);
	}
	FileEntry e;
	e.pathfile_buff = (char*)malloc(pathfile.len+1);
	memcpy(e.pathfile_buff, pathfile.str, pathfile.len);
	e.pathfile_buff[pathfile.len] = 0;
	e.pathfile = StringRange(e.pathfile_buff, pathfile.len);

	{
		const char* end = pathfile.str + pathfile.len;
		while (end != pathfile.str && *(end - 1) != '/') --end;
		e.path.str = e.pathfile_buff;
		e.path.len = end - pathfile.str;
	}

	{ // pick an identifier 'name' (for now dir/filename seems to suffice)
		const char* end = e.pathfile_buff + e.pathfile.len;
		while (end != e.pathfile_buff && *end != '/') --end;
		e.file.str = end + 1; // don't want the slash
		e.file.len = (e.pathfile_buff + e.pathfile.len) - end + 1;
		if (end != e.pathfile_buff) --end;
		while (end != e.pathfile_buff && *(end - 1) != '/') --end;
		e.name.str = end;
		e.name.len = (e.pathfile_buff + e.pathfile.len) - end;
	}
	e.shader_idx = shader_idx;
	file_entries.push_back(e);
	checkForUpdates(idx);
	return idx;
}

void injectProceduralFile(const char* pathfile, const char* text, size_t text_len) {
	int file_idx = getFileEntry(StringRange(pathfile), false);
	FileEntry& e = file_entries[file_idx];
	e.is_procedural = true;
	e.procedurally_modified = true;
	e.procedural_text = (char*)realloc(e.procedural_text, text_len + 1);
	e.procedural_text_len = text_len;
	ASSERT(e.procedural_text);
	e.procedural_text[text_len] = 0; // null term
	memcpy(e.procedural_text, text, text_len);
}

void FileEntry::addUser(int idx) {
	bool exists = false;
	for (int uidx : user_idxs)
		if (uidx == idx) {
			exists = true;
			break;
		}
	ASSERT(!exists);
	if (!exists) {//#OPT shouldn't need this check
		user_idxs.push_back(idx);
	}
}
void FileEntry::remUser(int idx) {
	for (unsigned i = 0; i < user_idxs.size(); ++i)
		if (user_idxs[i] == idx) {
			user_idxs[i] = user_idxs.back();
			user_idxs.pop_back();
			return;
		}
	ASSERT(false); // shouldn't be removing stuff that isn't there
}
static void clear(int file_idx) {
	FileEntry& f = file_entries[file_idx];
	if (f.text) free(f.text);
	f.text = 0;
	//for (int idx : f.include_idxs)
	//	file_entries[idx].remUser(file_idx);
	for (int i = 0; i < (int)f.includes.size(); ++i)
		file_entries[f.includes[i].file_idx].remUser(file_idx);
	//f.include_idxs.clear();
	f.includes.clear();
}
static void alertUsersOfChange(int file_idx) {
	if (file_entries[file_idx].shader_idx != -1) {
		shader_entries[file_entries[file_idx].shader_idx].stale = true;
		ASSERT(file_entries[file_idx].user_idxs.size() == 0); // shader main file shouldn't have users
	}
	else {
		for (int idx : file_entries[file_idx].user_idxs)
			alertUsersOfChange(idx);
	}
}
inline bool isWhitespace(char c) {
	return c == ' ';
}
inline bool isEndOfLine(char c) {
	return c == '\n';
}
inline bool isLowercaseAlpha(char c) {
	return c >= 'a' && c <= 'z';
}
inline bool isUppercaseAlpha(char c) {
	return c >= 'A' && c <= 'Z';
}
inline bool isAlpha(char c) {
	return isLowercaseAlpha(c) || isUppercaseAlpha(c);
}
inline void skipAlpha(const char** str, const char* str_end) {
	while (*str != str_end && isAlpha(**str)) *str += 1;
}
inline void skipUntilDoubleQuote(const char** str, const char* str_end) {
	while (*str != str_end && **str != '"') *str += 1;
}
inline void skipUntilWhitespaceOrEndOfLine(const char** str, const char* str_end) {
	while (*str != str_end && !isWhitespace(**str) && !isEndOfLine(**str)) *str += 1;
}
inline void skipWhitespace(const char** str, const char* str_end) {
	while(*str != str_end && isWhitespace(**str)) *str += 1;
}
inline bool isEndOfStr(const char* str, const char* str_end) {
	ASSERT(str <= str_end);
	return str >= str_end || str[0] == 0;
}
static void parseIncludes(int file_idx) {
	// Assumes all carriage returns have been removed, so we can use \n as our line break.
	const char* tex_start = file_entries[file_idx].text;
	const size_t len = file_entries[file_idx].text_len;
	const char* tex_end = tex_start + len;

	const char* tex = tex_start;
	bool is_start_of_line = true;
	int line = 1;
	StringRange include_directive = StringRange("include");
	while (!isEndOfStr(tex, tex_end)) {
		char c = *tex;
		if (c == '#') {
			Include inc;
			inc.line = line;
			inc.start = tex;
			tex += 1; // advance past the #
			StringRange directive;
			directive.str = tex;
			skipAlpha(&tex, tex_end);
			directive.len = tex - directive.str;
			if (directive == include_directive) {
				skipWhitespace(&tex, tex_end);
				if (*tex == '"') {
					tex += 1; // advance past "
					StringRange include_pathfile;
					include_pathfile.str = tex;
					skipUntilDoubleQuote(&tex, tex_end);
					if (*tex == '"') {
						// Got the closing quote, valid pathfile
						include_pathfile.len = tex - include_pathfile.str;
						tex += 1; // advance past the closing "
								  // Eat the rest of the line, if any, so that the entire line is notionally replaced with the file.
						skipWhitespace(&tex, tex_end);
						if (*tex == '\n') {
							line += 1;
							is_start_of_line = true;
							tex += 1;
						}
						inc.end = tex;
						// If the file we are parsing has a known path to it, prepend it to the included files.
						char full_pathfile[1024];
						if (file_entries[file_idx].path) {
							sprintf(full_pathfile, "%.*s%.*s", (int)file_entries[file_idx].path.len, file_entries[file_idx].path.str,
								(int)include_pathfile.len, include_pathfile.str);
						} else {
							sprintf(full_pathfile, "%.*s", (int)include_pathfile.len, include_pathfile.str);
						}
						StringRange r = StringRange(full_pathfile, file_entries[file_idx].path.len + include_pathfile.len);
						inc.file_idx = getFileEntry(r, false);
						file_entries[file_idx].includes.push_back(inc);
						file_entries[inc.file_idx].addUser(file_idx);
					}
				}
			}
			is_start_of_line = false;
		} else if (c == '\n') {
			line += 1;
			is_start_of_line = true;
			tex += 1;
		} else {
			is_start_of_line = false;
			tex += 1;
		}
	}
#if 0
	const FileEntry& file = file_entries[file_idx];
	print("Includes for file '%s':", file.pathfile.str);
	for (int i = 0; i < file.includes.size(); ++i) {
		print("%d: '%s'", file.includes[i].file_idx, file_entries[file.includes[i].file_idx].pathfile.str);
	}
#endif
}
static void checkForUpdates(int file_idx) {
	time_t last_modified = 0;
	if (file_entries[file_idx].is_procedural) {
		if (!file_entries[file_idx].procedurally_modified)
			return;
		file_entries[file_idx].procedurally_modified = false;
	}
	else {
		errno = 0;
		struct FileStats buf;
		int res = fileStat(file_entries[file_idx].pathfile.str, &buf);
		if (res != 0) {
			// do something on EACCES?
			clear(file_idx);
			if (!file_entries[file_idx].not_found)
				alertUsersOfChange(file_idx);
			file_entries[file_idx].not_found = true;
			return;
		}
		if (file_entries[file_idx].last_modified == buf.fs_mtime && !file_entries[file_idx].not_found)
			return;
		last_modified = buf.fs_mtime;
	}

	clear(file_idx);

	errno = 0;
	FILE* f = NULL;
	if (!file_entries[file_idx].is_procedural) {
#ifdef _WIN32
		fopen_s(&f, file_entries[file_idx].pathfile.str, "rb");
#else
		f = fopen(file_entries[file_idx].pathfile.str, "rb");
#endif
		if (!f) {
			// if we can't access the file, that's ok, just come back later (can be not found, or no access)
			return;
		}
		file_entries[file_idx].last_modified = last_modified;
	}

	file_entries[file_idx].not_found = false;

	size_t buff_len = 0;
	if (file_entries[file_idx].is_procedural) {
		buff_len = file_entries[file_idx].procedural_text_len;
	} else {
		fseek(f, 0, SEEK_END);
		buff_len = ftell(f);
		fseek(f, 0, SEEK_SET);
	}

	// This will be enough to hold whatever text we have, plus the null term.
	file_entries[file_idx].text = (char*)malloc(buff_len + 1);
	file_entries[file_idx].text[buff_len] = 0;

	if (file_entries[file_idx].is_procedural)
		memcpy(file_entries[file_idx].text, file_entries[file_idx].procedural_text, buff_len);
	else {
		fread(file_entries[file_idx].text, buff_len, 1, f);
		fclose(f);
	}

	// Strip the carriage returns. Not going to realloc, just waste the space they leave.
	{
		char* w = file_entries[file_idx].text;
		const char* r = file_entries[file_idx].text;
		const char* r_end = r + buff_len;
		while (r != r_end) {
			char c = *r++;
			if (c != '\r')
				*w++ = c;
		}
		file_entries[file_idx].text_len = w - file_entries[file_idx].text;
	}

	// Assumes all carriage returns have been removed.
	parseIncludes(file_idx);
	alertUsersOfChange(file_idx);
}
static void concatFinal(String* text, int file_idx, std::vector<int>* texts) {
	// Includes are in order of encounter. So we are always safe to write the current file into the text, up to the next include.
	auto includes = file_entries[file_idx].includes;
	const char* this_text = file_entries[file_idx].text;
	const char* this_text_end = file_entries[file_idx].text + file_entries[file_idx].text_len;
	int this_line = 1;
	char linebuff[256];
	// While there are includes, we write up until them, and then recurse to paste them in.
	for (int i = 0; i < (int)includes.size(); ++i) {
		StringRange r = StringRange(this_text, includes[i].start);	
		sprintf_s(linebuff, "#line %d \"%.*s\"\n", this_line, (int)file_entries[file_idx].name.len, file_entries[file_idx].name.str);
		text->append(linebuff);
		// Append the text up to the include directive
		text->append(r);
		// Append whatever text is to be pasted instead of the directive, unless we've already done so (implicit #pragma once)
		bool already_added = false;
		for (int t : *texts) {
			if (t == includes[i].file_idx) {
				already_added = true;
				break; 
			}
		}
		if (!already_added) {
			concatFinal(text, includes[i].file_idx, texts);
			text->append("\n");
		}
		// Skip past the directive in the source file
		this_text = includes[i].end;
		// Change the line directive to point to one line after this include directive.
		this_line = includes[i].line + 1;
	}
	// After includes are handled (if any), append the remaining part of the file.
	sprintf_s(linebuff, "#line %d \"%.*s\"\n", this_line, (int)file_entries[file_idx].name.len, file_entries[file_idx].name.str);
	text->append(linebuff);
	text->append(StringRange(this_text, this_text_end));
	texts->push_back(file_idx);
}
static void checkAllFilesForUpdates() {
	for (int i = 0; i < (int)file_entries.size(); ++i)
		checkForUpdates(i);
	for (ShaderEntry& s : shader_entries) {
		if (s.stale) {
			//PrintTimer t(file_entries[s.file_idx].name.str);
			s.stale = false;
			for (int idx : s.prog_idxs) {
				prog_entries[idx].try_relink = true;
				prog_entries[idx].stale = true;
			}
			s.log.clear();

			const FileEntry& f = file_entries[s.file_idx];
			if (f.text == 0) {
				continue;
			}

			s64 t_start = getTime();
			if (s.handle == 0) s.handle = glCreateShader(s.type);

			// concatenate everything into a single file
			s.final_text.clear();
			s.final_text.append(StringRange("#version 430 core\n"));

			// This basically works, except that it's a bit too good and not good enough.
			// It finds #includes inside #if WIN32 blocks and so it starts (successfully!) including files from the cpu src.
			// Which is sort of neat, in that maybe we can put .inl files in a shared dir (yay!)
			// And it means we can use #include in both cpu and gpu (yay!)
			// But if the CPU side wants to #include something unique, such as <stdint> or some such, then it wrongfully includes that too, even on GPU, cause it can't understand the #ifdef.
			// So that means .inl files could no longer have #includes inside the #ifWIN32 blocks, basically.
			// In practice this fails for "noise" file and rngeasy file if we use it
			std::vector<int> appended_texts;
			concatFinal(&s.final_text, s.file_idx, &appended_texts);
			/*
			{
				char pathfile[256];
				sprintf(pathfile, "final_shaders/%.*s", (int)file_entries[s.file_idx].file.len, file_entries[s.file_idx].file.str);
				FILE* f = fopen(pathfile, "wb");
				fwrite(s.final_text.str, s.final_text.len, 1, f);
				fclose(f);
			}
			*/

			// submit the final shader source to compile
			//glShaderSource(s.handle, texts.size(), &texts[0], &lens[0]);
			GLint final_len = (int)s.final_text.len;
			glShaderSource(s.handle, 1, &s.final_text.str, (const GLint*)&final_len);

			glCompileShader(s.handle);
			s.time_to_compile = getTime() - t_start;

#if 0
			if (s.source_dump_location) {
				char*& src = *s.source_dump_location;
				int total_len = 0;
				for (unsigned i = 0; i < texts.size(); ++i)
					total_len += lens[i];
				src = (char*)realloc(src, total_len+1);
				char* str = src;
				for (unsigned i = 0; i < texts.size(); ++i) {
					memcpy(str, texts[i], lens[i]);
					str += lens[i];
				}
				src[total_len] = 0;
			}
#endif

			int params = -1;
			glGetShaderiv(s.handle, GL_COMPILE_STATUS, &params);
			if (params != GL_TRUE) {
				int log_bytes = 0;
				glGetShaderiv(s.handle, GL_INFO_LOG_LENGTH, &log_bytes);
				s.log.reservebytes(log_bytes);
				int log_len = 0;
				glGetShaderInfoLog(s.handle, (int)s.log.max_bytes, &log_len, s.log.str);
				s.log.len = log_len;
				print("Shader Compile Error:\n%.*s", (int)s.log.len, s.log.str);
				glDeleteShader(s.handle);
				s.handle = 0;
				guiSetErroredFile(s.file_idx);
				any_errors_since_last_check = true;
			} 
		}
	}
	for (int i = 0; i < (int)prog_entries.size(); ++i) {
		ProgEntry& p = prog_entries[i];
		if (p.try_relink) {
			//PrintTimer t(p.name.str);
			p.try_relink = false;
			p.valid = false;
			bool any_zero = false;
			int attach_idxs[] = { p.vert_idx, p.frag_idx, p.comp_idx };
			for (unsigned a = 0; a < sizeof(attach_idxs) / sizeof(attach_idxs[0]); ++a)
				if (attach_idxs[a] != -1)
					if (shader_entries[attach_idxs[a]].handle == 0)
						any_zero = true;
			if (any_zero) 
				continue;

			s64 t_start = getTime();
			int new_handle = glCreateProgram();

			for (unsigned a = 0; a < sizeof(attach_idxs) / sizeof(attach_idxs[0]); ++a)
				if (attach_idxs[a] != -1)
					glAttachShader(new_handle, shader_entries[attach_idxs[a]].handle);

			glLinkProgram(new_handle);
			p.time_to_link = getTime() - t_start;

			for (unsigned a = 0; a < sizeof(attach_idxs) / sizeof(attach_idxs[0]); ++a)
				if (attach_idxs[a] != -1)
					glDetachShader(new_handle, shader_entries[attach_idxs[a]].handle);

			p.log.clear();

			int params = -1;
			glGetProgramiv(new_handle, GL_LINK_STATUS, &params);
			if (params == GL_TRUE) {
				if (p.handle) glDeleteProgram(p.handle);
				p.handle = new_handle;
				int valid = -1;
				glGetProgramiv(p.handle, GL_VALIDATE_STATUS, &valid);
				p.valid = valid == GL_TRUE;
				p.stale = false;
				p.rebuild_occurred = true;
			}
			else {
				print("Shader Linker Error.");
				int log_bytes = 0;
				glGetProgramiv(new_handle, GL_INFO_LOG_LENGTH, &log_bytes);
				p.log.reservebytes(log_bytes);
				int log_len = 0;
				glGetProgramInfoLog(new_handle, (int)p.log.max_bytes, &log_len, p.log.str);
				p.log.len = log_len;
				glDeleteProgram(new_handle);
				guiSetErroredProg(i);
				any_errors_since_last_check = true;
			}
		}
	}
}
int getShaderEntry(const char* pathfile) {
	if (!pathfile) return -1;
	int file_idx = getFileEntry(StringRange(pathfile), true);
	return file_entries[file_idx].shader_idx;
}
int getProgEntry(const char* vert, const char* frag, const char* comp) {
	int vert_idx = getShaderEntry(vert);
	int frag_idx = getShaderEntry(frag);
	int comp_idx = getShaderEntry(comp);
	for (int i = 0; i < (int)prog_entries.size(); ++i) {
		bool hit = true;
		hit &= (vert_idx == -1) || prog_entries[i].vert_idx == vert_idx;
		hit &= (frag_idx == -1) || prog_entries[i].frag_idx == frag_idx;
		hit &= (comp_idx == -1) || prog_entries[i].comp_idx == comp_idx;
		if (hit)
			return i;
	}
	ProgEntry e;
	int idx = (int)prog_entries.size();
	e.vert_idx = vert_idx;
	e.frag_idx = frag_idx;
	e.comp_idx = comp_idx;
	int idxs[] = { vert_idx, frag_idx, comp_idx };
	int valid_count = 0;
	for (unsigned i = 0; i < ARRSIZE(idxs); ++i)  {
		if (idxs[i] != -1) {
			if (valid_count != 0) e.name.append(" + ");
			e.name.append(file_entries[shader_entries[idxs[i]].file_idx].name);
			shader_entries[idxs[i]].prog_idxs.push_back(idx);
			valid_count += 1;
		}
	}
	prog_entries.push_back(e);
	return idx;
}
static void programStats(ProgramStats* stats, int prog_idx) {
	if (!stats) return;
	ProgEntry& e = prog_entries[prog_idx];
	stats->time_to_link = (float)secFromTime(e.time_to_link);
	stats->time_to_compile = 0.f;
	stats->rebuild_occurred = e.rebuild_occurred;
	stats->program_handle = e.handle;
	e.rebuild_occurred = false;
	int idxs[] = { e.vert_idx, e.frag_idx, e.comp_idx };	
	for (unsigned i = 0; i < ARRSIZE(idxs); ++i)  {
		if (idxs[i] != -1) {
			stats->time_to_compile += (float)secFromTime(shader_entries[idxs[i]].time_to_compile);
		}
	}
	if (e.comp_idx != -1 && !shader_entries[e.comp_idx].log.empty()) 
		stats->comp_log = shader_entries[e.comp_idx].log.str;
	else
		stats->comp_log = 0;
}
int getProgram(const char* vert, const char* frag, ProgramStats* stats)
{
	int prog_idx = getProgEntry(vert, frag, NULL);
	checkAllFilesForUpdates(); // could potentially limit this to just the files "vert" and "frag" care about, but it's probably not worth it right now (if you're changing files, you're probably changing the ones you care about here anyway, and those that aren't changing early out on stat)
	if (prog_entries[prog_idx].handle) {
		programStats(stats, prog_idx);
		return prog_entries[prog_idx].handle;
	} else {
		return 0;
	}
}
int useProgram(const char* vert, const char* frag, ProgramStats* stats) {
	int prog_idx = getProgEntry(vert, frag, NULL);
	checkAllFilesForUpdates(); // could potentially limit this to just the files "vert" and "frag" care about, but it's probably not worth it right now (if you're changing files, you're probably changing the ones you care about here anyway, and those that aren't changing early out on stat)
	if (prog_entries[prog_idx].handle) {
		programStats(stats, prog_idx);
		glUseProgram(prog_entries[prog_idx].handle);
		return prog_entries[prog_idx].handle;
	} else {
		return 0;
	}
}
int useProgram(const char* comp, ProgramStats* stats) {
	int prog_idx = getProgEntry(NULL, NULL, comp);
	checkAllFilesForUpdates(); // could potentially limit this to just the files "vert" and "frag" care about, but it's probably not worth it right now (if you're changing files, you're probably changing the ones you care about here anyway, and those that aren't changing early out on stat)
	programStats(stats, prog_idx);
	if (prog_entries[prog_idx].handle) {
		glUseProgram(prog_entries[prog_idx].handle);
		return prog_entries[prog_idx].handle;
	}
	else {
		return 0;
	}
}

/* GUI Code */
#if SHADER_GUI
#include "imgui/imgui.h"
static int s_file_idx = -1;
static int s_prog_idx = 0;
static int s_include_idx = -1;
#if 0 // need to remove tempstr dependency
static void dumpFinalShaders(const char* directory) {
	for (int i = 0; i < (int)file_entries.size(); ++i) {
		FileEntry& f = file_entries[i];
		TempStr pathfile = TempStr("%s%.*s", directory, f.file.len, f.file.str);
		if (f.is_procedural) { // dump procedural files
			FILE* file = fopen(pathfile, "wb");
			if (file) {
				fwrite(f.procedural_text, f.procedural_text_len, 1, file);
				fclose(file);
			}
		}
		if (f.shader_idx != -1) { // dump final shader files, and their errors
			ShaderEntry& s = shader_entries[f.shader_idx];
			{
				FILE* file = fopen(pathfile, "wb");
				if (file) {
					fwrite(s.final_text.str, s.final_text.len, 1, file);
					fclose(file);
				}
			}
			if (!s.log.empty()) {
				FILE* file = fopen(TempStr("%s.shader_errors", pathfile.str), "wb");
				if (file) {
					fwrite(s.log.str, s.log.len, 1, file);
					fclose(file);
				}
			}
		}
	}
	for (int i = 0; i < (int)prog_entries.size(); ++i) { // dump program errors
		ProgEntry& p = prog_entries[i];
		if (!p.log.empty()) {
			FILE* file = fopen(TempStr("%s%02d.program_errors", directory, i), "wb");
			if (file) {
				fprintf(file, "Composed from %.*s\n", (int)p.name.len, p.name.str);
				fwrite(p.log.str, p.log.len, 1, file);
				fclose(file);
			}
		}
	}
}
#endif
static void guiSetErroredFile(int idx) { s_file_idx = idx; }
static void guiSetErroredProg(int idx) { s_prog_idx = idx; }
void guiIncludeTree(int file_idx) {
        gui::Text("%s", file_entries[file_idx].pathfile.str);
	gui::Indent();
	for (int idx : file_entries[file_idx].user_idxs)
		guiIncludeTree(idx);
	gui::Unindent();
}
void guiShader(bool* open) {
	const vec3 col_red = vec3(0.9f, 0.1f, 0.1f);
	const vec3 col_orange = vec3(1.0f, 0.6f, 0.1f);
	const vec3 col_pink = vec3(0.9f, 0.1f, 1.0f);
	const vec3 col_grey = vec3(0.5f);

	bool any_issues = false;
	for (const ShaderEntry& e : shader_entries)
		any_issues |= e.handle == 0;
	for (const ProgEntry& e : prog_entries)
		any_issues |= e.handle == 0;

	if (any_issues) {
		gui::PushStyleColor(ImGuiCol_TitleBg, vec4(col_red*0.8f, 1.f));
		gui::PushStyleColor(ImGuiCol_TitleBgActive, vec4(col_red*0.8f, 1.f));
		gui::PushStyleColor(ImGuiCol_TitleBgCollapsed, vec4(col_red*0.9f, 1.f));
	}
	if (gui::Begin("Shaders", open)) {
		gui::Text("Shaders");
		gui::SameLine();
		static char dump_directory[1024] = "./debug_shaders/";
		// if (gui::Button("Dump Final Shader Files To Directory")) {
		// 	dumpFinalShaders(dump_directory);
		// }
		gui::SameLine();
		gui::InputText("##dump_directory", dump_directory, sizeof(dump_directory));

		gui::Columns(2, "files", true);
		gui::SetColumnWidth(0, 400.f);
		gui::Separator();
		vec3 def_col = vec4(gui::GetStyleColorVec4(ImGuiCol_Text)).xyz();

		for (int i = 0; i < (int)file_entries.size(); ++i) {
			const FileEntry& f = file_entries[i];
			if (f.shader_idx == -1) continue;
			vec3 col = def_col;
			if (f.text == NULL) {
				col = col_grey;
			}
			else {
				const ShaderEntry& s = shader_entries[f.shader_idx];
				if (s.handle == 0) col = col_red;
			}

			gui::PushStyleColor(ImGuiCol_Text, vec4(col, 1.f));
			if (gui::Selectable(file_entries[i].pathfile.str, i == s_file_idx))
				s_file_idx = i;
			gui::PopStyleColor(1);
		}
		gui::NextColumn();
		if (s_file_idx >= 0 && s_file_idx < (int) file_entries.size()) {
			const FileEntry& f = file_entries[s_file_idx];
			gui::Text("Compile time: %3.2fms", msecFromTime(shader_entries[f.shader_idx].time_to_compile));
			if (!shader_entries[f.shader_idx].log.empty())
				gui::Text("%s", shader_entries[f.shader_idx].log.str);
			else
				gui::Text("No problem.");

		}
		gui::NextColumn();
		gui::Columns(1);
		gui::Separator();

		gui::Spacing();
		gui::Text("Programs");
		gui::Columns(2, "progs", true);
		gui::SetColumnWidth(0, 400.f);
		gui::Separator();
		for (int i = 0; i < (int)prog_entries.size(); ++i) {
			const ProgEntry& p = prog_entries[i];
			vec3 col = def_col;
			if (p.handle == 0) {
				col = col_red;
			}
			else {
				if (p.stale) {
					col = col_orange;
				}
				else if (!p.valid) {
					col = col_pink;
				}
			}
			gui::PushStyleColor(ImGuiCol_Text, vec4(col, 1.f));
			if (gui::Selectable(p.name.str, i == s_prog_idx))
				s_prog_idx = i;
			gui::PopStyleColor(1);
		}
		gui::NextColumn();
		if (prog_entries.size()) {
			const ProgEntry& p = prog_entries[s_prog_idx];
			gui::Text("Link time: %3.2fms", msecFromTime(p.time_to_link));
			if (p.stale || p.handle == 0) {
				gui::Text("Shaders aren't valid");
				if (p.log.len)
					gui::Text("%s", p.log.str);
			}
			else {
				if (!p.log.empty())
                                        gui::Text("%s", p.log.str);
				else
					gui::Text("No problem.");
			}
		}
		gui::NextColumn();
		gui::Columns(1);
		gui::Separator();

		gui::Spacing();
		gui::Text("Includes");
		gui::Columns(2, "includes", true);
		gui::SetColumnWidth(0, 400.f);
		gui::Separator();
		for (int i = 0; i < (int)file_entries.size(); ++i) {
			const FileEntry& f = file_entries[i];
			if (f.shader_idx != -1) continue;
			vec3 col = def_col;
			if (f.text == NULL) {
				col = col_grey;
			}
			gui::PushStyleColor(ImGuiCol_Text, vec4(col, 1.f));
			if (gui::Selectable(file_entries[i].pathfile.str, i == s_include_idx))
				s_include_idx = i;
			gui::PopStyleColor(1);
		}
		gui::NextColumn();
		if (s_include_idx >= 0 && s_include_idx < (int) file_entries.size()) {
			gui::Text("Files using this one:");
			guiIncludeTree(s_include_idx);
		}
		gui::NextColumn();
		gui::Columns(1);
		gui::Separator();

	} gui::End();
	if (any_issues)
		gui::PopStyleColor(3);
}
void requestFinalShaderSource(const char* pathfile, char** source_location) {
	int e = getShaderEntry(pathfile);
	shader_entries[e].source_dump_location = source_location;
}
bool checkForShaderErrors() {
	bool ret = any_errors_since_last_check;
	any_errors_since_last_check = false;
	return ret;
}

#else
void guiSetErroredFile(int idx) { }
void guiSetErroredProg(int idx) { }
#endif