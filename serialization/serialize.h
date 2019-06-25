#pragma once
#include "core/crc.h"
#include <list>
#include <vector>


/* notes


*/

#define SERIALIZE_TYPE_ARRAY 0
#define SERIALIZE_TYPE_STRUCT 1
#define SERIALIZE_TYPE_POD 2
#define SERIALIZE_TYPE_NAME 3

#define TYPE_NAME_LEN 31
#define VAR_NAME_LEN 255
#define CRC_SIZE 4
#define FULL_SIZE_BITS 56

struct EntryHeader {
	u8 kind : 3;
	u8 type_len : 5;  // length of the string that stores type. 0 length means builtin c type, which means the type_name stores a byte of that type
};

typedef u64 LargeSize;
typedef u64 ByteStreamIdx;
typedef u16 StringDescIdx;

inline int highest_bit(LargeSize s) {
	int i = 0;
	for (; s; s >>= 1, i++);
	return i;
}

inline bool isStr(const char* str1, size_t len1, const char* str2, size_t len2) {
	if (len1 != len2) return false;
	for (int i = 0; i < len1; ++i) 
		if (str1[i] != str2[i])
			return false;
	return true;
}

#define STRBUFF_MAX (VAR_NAME_LEN+1) // max varname length + null term
struct StringDesc {
	u8 type;
	u32 idx;
	u8 len;
};

struct ByteStream {
	std::vector<char> str_bytes;
	std::vector<StringDesc> str_descs;
	std::vector<char> bytes;
	std::vector<std::vector<StringDescIdx> > str_hash_buckets;
	int str_hash_bucket_count = 257;
	ByteStreamIdx read_idx = 0;
	char strbuff[STRBUFF_MAX];

	ByteStream() {
		str_hash_buckets.resize(str_hash_bucket_count);
	}

	inline u32 key_to_idx(u8 type, const char* str, int len) {
		u32 h = crc32(0xdeadbeef, str, len);
		h ^= type;
		return h%str_hash_buckets.size();
	}
	inline void strHashAdd(u8 type, const char* str, int len, StringDescIdx value) {
		u32 idx = key_to_idx(type,str,len);
		str_hash_buckets[idx].push_back(value);
	}
	inline bool strHashGet(u8 type, const char* str, int len, StringDescIdx& val) {
		u32 idx = key_to_idx(type,str,len);
		const char* str_ptr = str_bytes.size() ? &str_bytes[0] : NULL;
		if (str_hash_buckets[idx].size()) {
			for (const StringDescIdx& i : str_hash_buckets[idx]) {
				const StringDesc& d = str_descs[i];
				if (type == d.type && isStr(str,len, str_ptr+d.idx,d.len)) {
					val = i;
					return true;
				}
			}
		}
		return false;
	}

	inline StringDescIdx strDescIdx(u8 type, const char* str, int len = 0) {
		if (len == 0) len = (int)strlen(str);
		assert(len < 256); // only 8 byte length supported. who names things longer than that, come on

		StringDescIdx desc_idx;
		if (strHashGet(type, str, len, desc_idx)) {
			return desc_idx;
		}

		u32 idx = (u32)str_bytes.size();
		str_bytes.reserve(str_bytes.size()+len);
		for (int i = 0; i < len; ++i)
			str_bytes.push_back(str[i]);
		StringDesc new_desc = {type,idx,(u8)len};
		strHashAdd(type, str, len, (StringDescIdx)str_descs.size());
		str_descs.push_back(new_desc);
		return StringDescIdx(str_descs.size()-1);
	}
	inline void SerializeStringTable(bool read) {
		if (read) {
			StringDescIdx str_descs_count = 0;
			doPOD(&str_descs_count, true);
			if (str_descs_count != 0) {
				str_descs.resize(str_descs_count);
				doMem(&str_descs[0], str_descs_count*sizeof(StringDesc), true);
				u32 str_count = 0;
				doPOD(&str_count, true);
				str_bytes.resize(str_count);
				doMem(&str_bytes[0], str_count*sizeof(char), true);
			}
		} else {
			std::vector<char> temp_bytes = bytes; //#OPT oh god
			bytes.clear();
			StringDescIdx str_descs_count = (StringDescIdx)str_descs.size();
			doPOD(&str_descs_count, false);
			if (str_descs_count != 0) {
				doMem(&str_descs[0], str_descs_count*sizeof(StringDesc), false);
				u32 str_count = (u32)str_bytes.size();
				doPOD(&str_count, false);
				doMem(&str_bytes[0], str_count*sizeof(char), false);
			}
			bytes.insert(bytes.end(), temp_bytes.begin(), temp_bytes.end());
		}
	}
	template<typename T>
	inline void doPOD(T* v, bool read) {
		if (read) {
			size_t s = sizeof(T);
			memcpy(v, &bytes[read_idx], s);
			read_idx += s;
		} else {
			size_t s = sizeof(T);
			bytes.resize(bytes.size()+s);
			memcpy(&bytes[bytes.size()-s], v, s);
		}
	}
	inline void doMem(void* m, size_t s, bool read) {
		if (read) {
			memcpy(m, &bytes[read_idx], s);
			read_idx += s;
		} else {
			bytes.resize(bytes.size()+s);
			memcpy(&bytes[bytes.size()-s], m, s);
		}
	}
	template<typename T>
	inline void doSize(T* s, int num_bits, bool read, ByteStreamIdx where = 0) {
		if (read) {
			T w = 0;
			int chunk = 0;
			while (true) {
				char b = bytes[read_idx];
				w |= T(b&0x7f) << (chunk*7);
				read_idx += 1;
				chunk += 1;
				if (b&0x80) {
					// keep reading
				} else {
					break;
				}
			}
			*s = w;
		} else {
			//ByteStreamIdx curr = bytes.size();
			T w = *s;
	
			if (num_bits == 0) num_bits = max(1, highest_bit(w));
			//log("%llu needs %d bits\n", w, num_bits);
			if (num_bits > (64-8)) {
				assert(false); // need a bit on each of the first 7 bytes
				//log("size too large\n");
				w = 0; // #WRONG ...pray nobody needs a size this large
			}

			while(num_bits > 0) {
				char b = w & 0x7f;
				if (num_bits > 7) {
					b |= 0x80; // high bit signifies next byte is needed
					w = w >> 7;
					num_bits -= 7;
				} else {
					num_bits = 0;
				}
				//#DODGY where==0 is assumed to be a sentinel, but that's just because we never write size at byte index 0...
				if (where) bytes[where++] = b;
				else bytes.push_back(b);
			}
			//log("wrote %d bytes\n", bytes.size()-curr);
		}
	}
	inline u16 doString(u8 type, const char* v, size_t len, bool read) {
		if (read) {
			StringDescIdx idx = 0;
			doSize(&idx, 0, true);
			return idx;
		} else {
			StringDescIdx idx = strDescIdx(type, v, (int)len);
			doSize(&idx, 0, false);
			return idx;
		}
	}
	inline bool readStringAndMatch(u8 type, const char* str, size_t len) {
		if (str_descs.size() == 0) return false;
		u16 idx = doString(type, str,len, true);
		return type == str_descs[idx].type && ::isStr(&str_bytes[0]+str_descs[idx].idx, str_descs[idx].len, str, len);
	}
	inline bool doCRC(bool read) {
		u32 magic = 0x1337c0de + 3;
		if (read) {
			if (bytes.size()<5) return false;
			u32 crc_expected = crc32(magic, &bytes[0], bytes.size()-sizeof(u32));
			u32 crc;
			ByteStreamIdx idx_curr = read_idx;
			read_idx = bytes.size()-sizeof(u32);
			doPOD(&crc, true);
			read_idx = idx_curr;
			if (crc != crc_expected) {
				log("Incorrect crc %08x expected, got %08x\n", crc_expected, crc);
			}
			return crc == crc_expected;
		} else {
			u32 crc = crc32(magic, &bytes[0], bytes.size());
			doPOD(&crc, false);
			return true;
		}
	}
	inline ByteStreamIdx endIdx() {
		return bytes.size()-CRC_SIZE;
	}
};

inline EntryHeader makeHeader(u8 kind, const char* type_name) {
	EntryHeader header;
	assert(kind < 8);
	header.kind = kind;
	size_t len = strlen(type_name);
	assert(len <= size_t(TYPE_NAME_LEN));
	header.type_len = min(len, size_t(TYPE_NAME_LEN));
	return header;
}

template<typename T> inline float Floatify(const T& v) { (void)v; return -42.f; }
template<> inline float Floatify<int>(const int& v) { return (float)v; }
template<> inline float Floatify<float>(const float& v) { return v; }
template<> inline float Floatify<bool>(const bool& v) { return v ? 1.f : 0.f; }

enum {SERIALIZE_MATCH, SERIALIZE_MISMATCH, SERIALIZE_END_OF_SCOPE};
template<typename T>
inline int SerializeBasic(ByteStream& bytes, bool read, const char* type_name, const char* var_name, ByteStreamIdx scope_end, LargeSize& type_bytes, ByteStreamIdx& size_idx) { //, LargeSize& arr_bytes, ByteStreamIdx& arr_size_idx) {
	if (read) {
		if (scope_end != 0 && bytes.read_idx >= scope_end) {
			assert(bytes.read_idx == scope_end);
			//log("Skipping entry [%s %s] - No more bytes in scope!\n", type_name, var_name);
			return SERIALIZE_END_OF_SCOPE;
		}
		if (bytes.read_idx >= bytes.endIdx()) {
			assert(bytes.read_idx == bytes.endIdx());
			//log("Skipping entry [%s %s] - No more bytes in file!\n", type_name, var_name);
			return SERIALIZE_END_OF_SCOPE;
		}

		bool is_type = bytes.readStringAndMatch(SerializeType<T>(), type_name, strlen(type_name));
		bool is_var = bytes.readStringAndMatch(SERIALIZE_TYPE_NAME, var_name, strlen(var_name));
		size_idx = bytes.read_idx;
		bytes.doSize(&type_bytes, 0, true);
		bool is_size = SerializeSizeBytes<T>() == 0 || (type_bytes == SerializeSizeBytes<T>()); // if we know the size, might as well check it

		if (is_type && is_var && is_size) {
			return SERIALIZE_MATCH; // TODO: check pod bytes somehow
		} else {
			//log("Skipping %ld bytes entry [%s %s] - type %s, var %s\n", type_bytes, type_name, var_name, is_type ? "ok" : "BAD", is_var ? "ok" : "BAD");
			bytes.read_idx += type_bytes;
			return SERIALIZE_MISMATCH;
		}
	} else {
		bytes.doString(SerializeType<T>(), type_name, strlen(type_name), false);
		bytes.doString(SERIALIZE_TYPE_NAME, var_name, strlen(var_name), false);
		size_idx = bytes.bytes.size();
		type_bytes = SerializeSizeBytes<T>();
		//ByteStreamIdx idx = bytes.bytes.size();
		bytes.doSize(&type_bytes, (type_bytes == 0 ? FULL_SIZE_BITS : 0), false);
		//log("wrote variable %d bytes\n", bytes.bytes.size()-idx);
		//log("%s %s - %d bytes\n", type_name, var_name, SerializeSizeBytes<T>());

		return SERIALIZE_MATCH;
	}
}
inline void WriteStructBytes(ByteStream& bytes, LargeSize size, ByteStreamIdx size_idx) {
	//log("writing back size %ld to idx %ld\n", size, size_idx);
	memcpy(&bytes.bytes[size_idx], &size, sizeof(LargeSize));
}

template<typename A>
inline void SerializeCoreArray(ByteStream& bytes, bool read, A& d, ByteStreamIdx scope_start, ByteStreamIdx scope_end) {
	if (read) {
		d.clear();
		bool homogenous = true;
		bytes.doPOD(&homogenous, true);
		LargeSize element_size = 0;
		while (bytes.read_idx < scope_end) {
			if (!homogenous || element_size == 0) //if not homogenous, or if we haven't read the first size yet
				bytes.doPOD(&element_size, true);
			ByteStreamIdx sub_scope_end = bytes.read_idx + element_size;
			SerializeCore(bytes, read, d.push(), bytes.read_idx, sub_scope_end);
			bytes.read_idx = sub_scope_end;
		}
	} else {
		ByteStreamIdx start_idx = bytes.bytes.size();
		// take the spot for homogenous flag
		bool homogenous = true;

try_again:

		bytes.doPOD(&homogenous, false);
		LargeSize first_element_size = 0;
		for (int i = 0; i < d.count; ++i) {
			ByteStreamIdx element_size_idx = bytes.bytes.size();
			LargeSize element_size = 0; // take the spot for size
			if (!homogenous || i == 0) {
				bytes.doPOD(&element_size, false);
			}

			ByteStreamIdx write_start_idx = bytes.bytes.size(); 
			SerializeCore(bytes, read, d[i], scope_start, scope_end);
			element_size = bytes.bytes.size() - write_start_idx;

			if (i == 0) {
				first_element_size = element_size;
			}

			if (homogenous && (first_element_size != element_size)) {
				// if any element is a different size than the first, restart and mark as non-homogenous
				bytes.read_idx = start_idx;
				bytes.bytes.resize(start_idx);
				homogenous = false;
				goto try_again;
			}
						
			if (!homogenous || i == 0) { // write the size back, now that we know it
				memcpy(&bytes.bytes[element_size_idx], &element_size, sizeof(LargeSize));
			}
		}
	}
}
template<typename T>
inline void SerializeCore(ByteStream& bytes, bool read, Bunch<T>& d, ByteStreamIdx scope_start, ByteStreamIdx scope_end) {
	SerializeCoreArray(bytes, read, d, scope_start, scope_end);
}
template<typename T, int C>
inline void SerializeCore(ByteStream& bytes, bool read, Bin<T,C>& d, ByteStreamIdx scope_start, ByteStreamIdx scope_end) {
	SerializeCoreArray(bytes, read, d, scope_start, scope_end);
}

template<typename T>
inline void SerializeMember(ByteStream& bytes, bool read, T& d, const char* type_name, const char* var_name, ByteStreamIdx scope_start, ByteStreamIdx scope_end) {
	ByteStreamIdx search_start = bytes.read_idx;
	//log("Searching for [%s %s] starting at %ld\n", type_name, var_name, search_start);
	bool have_looped = false;
	while (true) {
		if (have_looped && bytes.read_idx == search_start)
			break;
		//else 
		//	log("[read %ld] [search start %ld] [scope start %ld]\n", bytes.read_idx, search_start, scope_start);
		LargeSize read_size = 0;
		ByteStreamIdx size_idx = 0;
		int res = SerializeBasic<T>(bytes, read, type_name, var_name, scope_end, read_size, size_idx);
		if (res == SERIALIZE_MATCH) {
			ByteStreamIdx sub_scope_start = bytes.read_idx;
			//log("Going into %s %s, [start %ld] [size %ld] [idx %ld]\n", type_name, var_name, sub_scope_start, read_size, bytes.read_idx);
			ByteStreamIdx sub_scope_end = sub_scope_start+read_size;
			//log("struct %s %s {\n", type_name, var_name);
			ByteStreamIdx write_start_idx = bytes.bytes.size();
			SerializeCore(bytes, read, d, sub_scope_start, sub_scope_end);
			if (!read && read_size == 0) {
				LargeSize size = bytes.bytes.size() - write_start_idx;
				bytes.doSize(&size, FULL_SIZE_BITS, false, size_idx);
				//WriteStructBytes(bytes, size, size_idx);
			}
			if (read) {
				bytes.read_idx = sub_scope_end;
			}
			//log("}\n");
			break;
		}
		if (res == SERIALIZE_END_OF_SCOPE) {
			//log("end of scope struct\n");
			bytes.read_idx = scope_start;
			have_looped = true;
			//log("going back to start %ld\n", scope_start);
		}
	}
}

inline void FileBytes(ByteStream& bytes, bool read, const char* filename) {
	if (read) {
		FILE* f = NULL;
		fopen_s(&f, filename, "rb");
		if (!f)
		{
			log("[Serialize] unable to open file '%s'\n", filename);
			return;
		}

		fseek(f, 0, SEEK_END);
		size_t length = ftell(f);
		fseek(f, 0, SEEK_SET);

		bytes.bytes.resize(length);
		if (bytes.bytes.size() == length) {
			fread(&bytes.bytes[0], length, 1, f);
		}
		fclose(f);
	} else {
		FILE* f = NULL;
		fopen_s(&f, filename, "wb");
		if (!f)
		{
			log("[Serialize] unable to open file '%s'\n", filename);
			return;
		}
		fwrite(&bytes.bytes[0], bytes.bytes.size(), 1, f);
		fflush(f);
		fclose(f);
	}
}

inline char itoh(char i) {
	assert(i<16);
	char t[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	return t[i];
}
inline char htoi(char h) {
	if ((h>='0'&&h<='9')) return h-'0';
	if ((h>='a'&&h<='f')) return 10+(h-'a');
	return -1;
}

inline size_t HexBytes(ByteStream& bytes, bool read, char* buff, size_t buff_size) {
	if (read) {
		if (buff_size&1) return 0; // evens only
		size_t read_count = 0;
		for (int i = 0; i < buff_size; i+=2) {	
			u8 lower = htoi(buff[i]);
			u8 upper = htoi(buff[i+1]);
			if (upper != -1 && lower != -1) {
				bytes.bytes.push_back((upper<<4)|lower);
				read_count++;
			}
		}
		return read_count;
	} else {
		if (buff_size < (bytes.bytes.size()*2)) {
			log("Not enough space for hex string\n");
			return 0;
		}
		for (int i = 0; i < bytes.bytes.size(); ++i) {
			u8 b = bytes.bytes[i];
			*(buff++) = itoh((b>>0)&0x0f);
			*(buff++) = itoh((b>>4)&0x0f);
		}
		return bytes.bytes.size()*2;
	}
}
template<typename T>
inline void SaveToFile(T& t, const char* type_name, const char* name, const char* filename) {
	ByteStream bytes;
	bytes.str_bytes.reserve(1024);
	bytes.str_descs.reserve(64);
	Serialize(bytes, false, t, type_name, name);
	bytes.SerializeStringTable(false);
	bytes.doCRC(false);
	FileBytes(bytes, false, filename);
}

template<typename T>
inline bool LoadFromFile(T& t, const char* type_name, const char* name, const char* filename) {
	ByteStream bytes;
	bytes.str_bytes.reserve(1024);
	bytes.str_descs.reserve(64);
	FileBytes(bytes, true, filename);
	if (bytes.doCRC(true)) {
		bytes.SerializeStringTable(true);
		Serialize(bytes, true, t, type_name, name);
		return true;
	}
	return false;
}
