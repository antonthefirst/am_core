#pragma once

#include <stddef.h>     /* For size_t */

struct ProgramStats {
	float time_to_compile = 0.f; // time in seconds it took to compile all shaders
	float time_to_link = 0.f;    // time it took to link the program
	const char* comp_log = 0;   
	bool rebuild_occurred = false; // program was rebuilt due to changes. true on first ever build too.
	int program_handle = 0;
};

void injectProceduralFile(const char* pathfile, const char* text, size_t text_len);
int getProgram(const char* vert, const char* frag, ProgramStats* stats = 0);
int useProgram(const char* vert, const char* frag, ProgramStats* stats = 0);
int useProgram(const char* comp, ProgramStats* stats = 0);

#define SHADER_GUI 1
#if SHADER_GUI
void guiShader(bool* open = 0);
void requestFinalShaderSource(const char* pathfile, char** source_location);
bool checkForShaderErrors();
#endif