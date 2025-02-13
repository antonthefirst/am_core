#pragma once

#include "core/basic_types.h"

// Not actually wang hash, just copy pasta from resource.cpp and copypasta from gpu hashes.
inline u32 WangHash(u32 b)
{
	b = (b ^ 0x8564231U) ^ (b >> 16U); //use a magic constant somewhere in the middle of u32 range, instead of 61
	b *= 9U;
	b = b ^ (b >> 4U);
	b *= 0x27d4eb2dU;
	b = b ^ (b >> 15U);
	return b;
}
inline float WangHashFloat(u32 seed)	{ return float(WangHash(seed) % 16777216)*(1.0f / 16777215.0f); }
inline float WangHashFloat(u32 seed, float val_min, float val_max) { return val_min + (val_max-val_min)*WangHashFloat(seed); }

inline float cubicPulse( float c, float w, float x ) {
	x = abs(x - c);
	if( x>w ) return 0.0f;
	x /= w;
	return 1.0f - x*x*(3.0f-2.0f*x);
}

// for noise.h
inline vec2 rotation2d(float a) { return vec2(cos(a), sin(a)); }
inline vec2 rotate(vec2 r, vec2 v) { return vec2(v.x*r.x - v.y*r.y, v.x*r.y + v.y*r.x); }
inline vec2 rotate(float r, vec2 v) { return rotate(rotation2d(r), v); }

inline void normalize(float & dx, float & dy)
{
	float il = 1.f/sqrtf(dx*dx + dy*dy);
	dx *= il;
	dy *= il;
}

// Migrate to mathlib
inline vec3 round(vec3 v) {
	return vec3(round(v.x), round(v.y), round(v.z));
}

inline vec2 st_from_id(uvec2 id) { return  vec2(id) + vec2(0.5); }
inline vec2 st_from_id(ivec2 id) { return  vec2(id) + vec2(0.5); }
inline ivec2 id_from_st(vec2 st) { return ivec2(st); } // should be -0.5. This can produce negative coordinates, but that is up to border to handle and clamp!
inline ivec2 id_from_uv(vec2 uv, vec2 res) { return ivec2(uv * res); }
inline vec2 xy_from_st(vec2 st, vec2 res) { return (2.0 * st - res) / res.y; }
inline vec2 st_from_xy(vec2 xy, vec2 res) { return ((xy * res.y) + res) * 0.5; }
inline vec2 xy_from_uv(vec2 uv) { return uv * 2.0 - 1.0; }
inline vec2 uv_from_xy(vec2 xy) { return xy * 0.5 + 0.5; }
inline vec2 uv_from_st(vec2 st, vec2 res) { return st / res; }
inline vec2 uv_from_id(ivec2 id, vec2 res) { return (vec2(id) + vec2(0.5)) / res; }
inline vec2 st_from_uv(vec2 uv, vec2 res) { return uv * res; }

inline vec3 st_from_id(ivec3 id) { return  vec3(id) + vec3(0.5); }
inline vec3 uvw_from_id(ivec3 id, vec3 res) { return (vec3(id) + vec3(0.5f)) / res; }
inline vec3 uv_from_st(vec3 st, vec3 res) { return st / res; }


inline vec2 perp(vec2 v) {
	return vec2(-v.y, v.x);
}
inline vec2 normalizeOrZero(vec2 v) {
	float l = length(v);
	return l > 0.0 ? v / l : vec2(0.0f);
}
inline vec2 normalizeOr(vec2 v, vec2 or_v) {
	float l = length(v);
	return l > 0.0 ? v / l : or_v;
}
inline vec3 normalizeOrZero(vec3 v) {
	float l = length(v);
	return l > 0.0 ? v / l : vec3(0.0f);
}
inline vec3 normalizeOr(vec3 v, vec3 or_v) {
	float l = length(v);
	return l > 0.0 ? v / l : or_v;
}

// Should really be some kind of "stdlib", not math.
template<typename T>
inline void swap(T& a, T& b) {
	T temp = a;
	a = b;
	b = temp;
}