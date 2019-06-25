#include "serialize.h"

template<typename T>
inline u8 SerializeSizeBytes() { return 0; }

template<typename T>
inline u8 SerializeType() { return SERIALIZE_TYPE_ARRAY; }

#define POD(T) \
template<> \
inline u8 SerializeSizeBytes<T>() { return sizeof(T); } \
template<> \
inline u8 SerializeType<T>() { return SERIALIZE_TYPE_POD; } \
inline void SerializeCore(ByteStream& bytes, bool read, T& d, ByteStreamIdx scope_start, ByteStreamIdx scope_end) { \
	(void)scope_start; (void)scope_end; \
	bytes.doPOD(&d, read); \
}

#define STRUCT(T) \
template<> \
inline u8 SerializeType<T>() { return SERIALIZE_TYPE_STRUCT; } \
void SerializeCore(ByteStream& bytes, bool read, T& d, ByteStreamIdx scope_start, ByteStreamIdx scope_end); \
void Serialize(ByteStream& bytes, bool read, T& d, const char* type_name, const char* var_name, ByteStreamIdx scope_start = 0, ByteStreamIdx scope_end = 0) { \
	SerializeMember(bytes, read, d, type_name, var_name, scope_start, scope_end); \
} \
void SerializeCore(ByteStream& bytes, bool read, T& d, ByteStreamIdx scope_start, ByteStreamIdx scope_end) {

#define END \
} \


#define VAR(VT,VN,INIT,...) SerializeMember(bytes, read, d.VN, #VT, #VN, scope_start, scope_end);

#define BUNCH(VT,VN,...) SerializeMember(bytes, read, d.VN, #VT, #VN, scope_start, scope_end);

#define BIN(VT,VN,COUNT,...) SerializeMember(bytes, read, d.VN, #VT, #VN, scope_start, scope_end);


POD(u8)
POD(s8)
POD(u16)
POD(s16)
POD(u32)
POD(s32)
POD(float)
POD(bool)
POD(char)
POD(vec2)
POD(vec3)
POD(vec4)