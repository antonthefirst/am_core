#pragma once
#include "math/maths.h"
#include "core/packing.h"

/* I don't think this is helping.
   Why?
struct ucol4 {
	unsigned int rgba;
};
*/

struct col3 {
	float r,g,b; 
	col3() { }
	col3(float _v) : r(_v), g(_v), b(_v) { }
	col3(float _r, float _g, float _b) : r(_r), g(_g), b(_b) { }
	col3 valueScale(const float s) const;
};
inline col3 col3::valueScale(const float s) const { return col3(r * s, g * s, b * s); }

struct col4 {
	float r,g,b,a;

	col4() { }
	col4(float _v, float _a = 1.0f) : r(_v), g(_v), b(_v), a(_a) { }
	col4(float _r, float _g, float _b, float _a = 1.0f) : r(_r), g(_g), b(_b), a(_a) { }
	col4(col3 c, float _a = 1.0f) : r(c.r), g(c.g), b(c.b), a(_a) { }
	col4(vec4 v) : r(v.x), g(v.y), b(v.z), a(v.w) { }
	col4(vec3 v) : r(v.x), g(v.y), b(v.z), a(1.0f) { }

	static col4 black();
	static col4 white();
	static col4 clear();

	col4 operator*(const float s) const;
	col4 operator/(const float s) const;
	col4 operator+(const col4& rhs) const;
	col4 operator-(const col4& rhs) const;
	col4 valueScale(const float s) const;
	col3 rgb() const;
	u32 rgba_packed() const;
};

inline col4 col4::black() { return col4{ 0.0f, 0.0f, 0.0f, 1.0f }; }
inline col4 col4::white() { return col4{ 1.0f, 1.0f, 1.0f, 1.0f }; }
inline col4 col4::clear() { return col4{ 0.0f, 0.0f, 0.0f, 0.0f }; }

inline col4 col4::operator*(const float s) const { return col4(r * s, g * s, b * s, a * s); }
inline col4 col4::operator/(const float s) const { return col4(r / s, g / s, b / s, a / s); }
inline col4 col4::operator+(const col4& rhs) const { return col4(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a); }
inline col4 col4::operator-(const col4& rhs) const { return col4(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a); }
inline col4 col4::valueScale(const float s) const { return col4(r * s, g * s, b * s, a); }
inline col3 col4::rgb() const { return col3(r,g,b); }
inline u32 col4::rgba_packed() const { return {pack_unorm(r,g,b,a)}; }
inline col4 lerp(const col4& a, const col4& b, float t) {
	return a + (b - a) * t;
}

/* Should really be shared with gpu... */
inline float srgb_from_lrgb(float val) {
	if (val < 0.0031308)
		val *= 12.92f;
	else
		val = 1.055f * pow(val, 1.0f / 2.4f) - 0.055f;
	return val;
}
inline float lrgb_from_srgb(float val) {
	if (val < 0.04045)
		val /= 12.92f;
	else
		val = pow((val + 0.055f) / 1.055f, 2.4f);
	return val;
}
inline col3 srgb_from_lrgb(col3 col) { return col3(srgb_from_lrgb(col.r), srgb_from_lrgb(col.g), srgb_from_lrgb(col.b)); }
inline col3 lrgb_from_srgb(col3 col) { return col3(lrgb_from_srgb(col.r), lrgb_from_srgb(col.g), lrgb_from_srgb(col.b)); }
inline col4 srgb_from_lrgb(col4 col) { return col4(srgb_from_lrgb(col.rgb()), col.a); }
inline col4 lrgb_from_srgb(col4 col) { return col4(lrgb_from_srgb(col.rgb()), col.a); }
