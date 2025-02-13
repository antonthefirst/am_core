#pragma once
#pragma warning( disable : 4146)
#include "maths_scalar.h"
struct ivec2;
struct ivec3;
struct ivec4;
struct uvec2;
struct uvec3;
struct uvec4;
struct vec2;
struct vec3;
struct vec4;
struct dvec2;
struct dvec3;
struct dvec4;
struct bvec2;
struct bvec3;
struct bvec4;
struct bvec2 {
	bool x;
	bool y;
	bvec2() {}
	bvec2(bool s):x(s), y(s) {}
	bvec2(bool x_in, bool y_in):x(x_in), y(y_in) {}
	inline bool& operator[](const int idx) {
		return ((bool*)this)[idx];
	}
};
struct bvec3 {
	bool x;
	bool y;
	bool z;
	bvec3() {}
	bvec3(bool s):x(s), y(s), z(s) {}
	bvec3(bool x_in, bool y_in, bool z_in):x(x_in), y(y_in), z(z_in) {}
	bvec3(bvec2 v, bool s):x(v.x), y(v.y), z(s) {}
	inline bool& operator[](const int idx) {
		return ((bool*)this)[idx];
	}
	inline bvec2 xy() const {
		return bvec2(x, y);
	}
};
struct bvec4 {
	bool x;
	bool y;
	bool z;
	bool w;
	bvec4() {}
	bvec4(bool s):x(s), y(s), z(s), w(s) {}
	bvec4(bool x_in, bool y_in, bool z_in, bool w_in):x(x_in), y(y_in), z(z_in), w(w_in) {}
	bvec4(bvec3 v, bool s):x(v.x), y(v.y), z(v.z), w(s) {}
	inline bool& operator[](const int idx) {
		return ((bool*)this)[idx];
	}
	inline bvec2 xy() const {
		return bvec2(x, y);
	}
	inline bvec3 xyz() const {
		return bvec3(x, y, z);
	}
};
struct vec2 {
	float x;
	float y;
	vec2() {}
	vec2(float s):x(s), y(s) {}
	vec2(float x_in, float y_in):x(x_in), y(y_in) {}
	vec2(const ivec2& v);
	vec2(const uvec2& v);
	vec2(const dvec2& v);
	inline float& operator[](const int idx) {
		return ((float*)this)[idx];
	}
};
struct vec3 {
	float x;
	float y;
	float z;
	vec3() {}
	vec3(float s):x(s), y(s), z(s) {}
	vec3(float x_in, float y_in, float z_in):x(x_in), y(y_in), z(z_in) {}
	vec3(vec2 v, float s):x(v.x), y(v.y), z(s) {}
	vec3(const ivec3& v);
	vec3(const uvec3& v);
	vec3(const dvec3& v);
	inline float& operator[](const int idx) {
		return ((float*)this)[idx];
	}
	inline vec2 xy() const {
		return vec2(x, y);
	}
};
struct vec4 {
	float x;
	float y;
	float z;
	float w;
	vec4() {}
	vec4(float s):x(s), y(s), z(s), w(s) {}
	vec4(float x_in, float y_in, float z_in, float w_in):x(x_in), y(y_in), z(z_in), w(w_in) {}
	vec4(vec2 xy, vec2 zw):x(xy.x), y(xy.y), z(zw.x), w(zw.y) { }
	vec4(vec3 v, float s):x(v.x), y(v.y), z(v.z), w(s) {}
	vec4(const ivec4& v);
	vec4(const uvec4& v);
	vec4(const dvec4& v);
	inline float& operator[](const int idx) {
		return ((float*)this)[idx];
	}
	inline vec2 xy() const {
		return vec2(x, y);
	}
	inline vec3 xyz() const {
		return vec3(x, y, z);
	}
};
struct dvec2 {
	double x;
	double y;
	dvec2() {}
	dvec2(double s):x(s), y(s) {}
	dvec2(double x_in, double y_in):x(x_in), y(y_in) {}
	dvec2(const ivec2& v);
	dvec2(const uvec2& v);
	dvec2(const vec2& v);
	inline double& operator[](const int idx) {
		return ((double*)this)[idx];
	}
};
struct dvec3 {
	double x;
	double y;
	double z;
	dvec3() {}
	dvec3(double s):x(s), y(s), z(s) {}
	dvec3(double x_in, double y_in, double z_in):x(x_in), y(y_in), z(z_in) {}
	dvec3(dvec2 v, double s):x(v.x), y(v.y), z(s) {}
	dvec3(const ivec3& v);
	dvec3(const uvec3& v);
	dvec3(const vec3& v);
	inline double& operator[](const int idx) {
		return ((double*)this)[idx];
	}
	inline dvec2 xy() const {
		return dvec2(x, y);
	}
};
struct dvec4 {
	double x;
	double y;
	double z;
	double w;
	dvec4() {}
	dvec4(double s):x(s), y(s), z(s), w(s) {}
	dvec4(double x_in, double y_in, double z_in, double w_in):x(x_in), y(y_in), z(z_in), w(w_in) {}
	dvec4(dvec3 v, double s):x(v.x), y(v.y), z(v.z), w(s) {}
	dvec4(const ivec4& v);
	dvec4(const uvec4& v);
	dvec4(const vec4& v);
	inline double& operator[](const int idx) {
		return ((double*)this)[idx];
	}
	inline dvec2 xy() const {
		return dvec2(x, y);
	}
	inline dvec3 xyz() const {
		return dvec3(x, y, z);
	}
};
struct ivec2 {
	int x;
	int y;
	ivec2() {}
	ivec2(int s):x(s), y(s) {}
	ivec2(int x_in, int y_in):x(x_in), y(y_in) {}
	ivec2(const uvec2& v);
	ivec2(const vec2& v);
	ivec2(const dvec2& v);
	inline int& operator[](const int idx) {
		return ((int*)this)[idx];
	}
};
struct ivec3 {
	int x;
	int y;
	int z;
	ivec3() {}
	ivec3(int s):x(s), y(s), z(s) {}
	ivec3(int x_in, int y_in, int z_in):x(x_in), y(y_in), z(z_in) {}
	ivec3(ivec2 v, int s):x(v.x), y(v.y), z(s) {}
	ivec3(const uvec3& v);
	ivec3(const vec3& v);
	ivec3(const dvec3& v);
	inline int& operator[](const int idx) {
		return ((int*)this)[idx];
	}
	inline ivec2 xy() const {
		return ivec2(x, y);
	}
};
struct ivec4 {
	int x;
	int y;
	int z;
	int w;
	ivec4() {}
	ivec4(int s):x(s), y(s), z(s), w(s) {}
	ivec4(int x_in, int y_in, int z_in, int w_in):x(x_in), y(y_in), z(z_in), w(w_in) {}
	ivec4(ivec3 v, int s):x(v.x), y(v.y), z(v.z), w(s) {}
	ivec4(const uvec4& v);
	ivec4(const vec4& v);
	ivec4(const dvec4& v);
	inline int& operator[](const int idx) {
		return ((int*)this)[idx];
	}
	inline ivec2 xy() const {
		return ivec2(x, y);
	}
	inline ivec3 xyz() const {
		return ivec3(x, y, z);
	}
};
struct uvec2 {
	unsigned int x;
	unsigned int y;
	uvec2() {}
	uvec2(unsigned int s):x(s), y(s) {}
	uvec2(unsigned int x_in, unsigned int y_in):x(x_in), y(y_in) {}
	uvec2(const ivec2& v);
	uvec2(const vec2& v);
	uvec2(const dvec2& v);
	inline unsigned int& operator[](const int idx) {
		return ((unsigned int*)this)[idx];
	}
};
struct uvec3 {
	unsigned int x;
	unsigned int y;
	unsigned int z;
	uvec3() {}
	uvec3(unsigned int s):x(s), y(s), z(s) {}
	uvec3(unsigned int x_in, unsigned int y_in, unsigned int z_in):x(x_in), y(y_in), z(z_in) {}
	uvec3(uvec2 v, unsigned int s):x(v.x), y(v.y), z(s) {}
	uvec3(const ivec3& v);
	uvec3(const vec3& v);
	uvec3(const dvec3& v);
	inline unsigned int& operator[](const int idx) {
		return ((unsigned int*)this)[idx];
	}
	inline uvec2 xy() const {
		return uvec2(x, y);
	}
};
struct uvec4 {
	unsigned int x;
	unsigned int y;
	unsigned int z;
	unsigned int w;
	uvec4() {}
	uvec4(unsigned int s):x(s), y(s), z(s), w(s) {}
	uvec4(unsigned int x_in, unsigned int y_in, unsigned int z_in, unsigned int w_in):x(x_in), y(y_in), z(z_in), w(w_in) {}
	uvec4(uvec3 v, unsigned int s):x(v.x), y(v.y), z(v.z), w(s) {}
	uvec4(const ivec4& v);
	uvec4(const vec4& v);
	uvec4(const dvec4& v);
	inline unsigned int& operator[](const int idx) {
		return ((unsigned int*)this)[idx];
	}
	inline uvec2 xy() const {
		return uvec2(x, y);
	}
	inline uvec3 xyz() const {
		return uvec3(x, y, z);
	}
};
inline vec2::vec2(const ivec2& v):x((float)v.x), y((float)v.y) {}
inline vec2::vec2(const uvec2& v):x((float)v.x), y((float)v.y) {}
inline vec2::vec2(const dvec2& v):x((float)v.x), y((float)v.y) {}
inline vec3::vec3(const ivec3& v):x((float)v.x), y((float)v.y), z((float)v.z) {}
inline vec3::vec3(const uvec3& v):x((float)v.x), y((float)v.y), z((float)v.z) {}
inline vec3::vec3(const dvec3& v):x((float)v.x), y((float)v.y), z((float)v.z) {}
inline vec4::vec4(const ivec4& v):x((float)v.x), y((float)v.y), z((float)v.z), w((float)v.w) {}
inline vec4::vec4(const uvec4& v):x((float)v.x), y((float)v.y), z((float)v.z), w((float)v.w) {}
inline vec4::vec4(const dvec4& v):x((float)v.x), y((float)v.y), z((float)v.z), w((float)v.w) {}
inline dvec2::dvec2(const ivec2& v):x((double)v.x), y((double)v.y) {}
inline dvec2::dvec2(const uvec2& v):x((double)v.x), y((double)v.y) {}
inline dvec2::dvec2(const vec2& v):x((double)v.x), y((double)v.y) {}
inline dvec3::dvec3(const ivec3& v):x((double)v.x), y((double)v.y), z((double)v.z) {}
inline dvec3::dvec3(const uvec3& v):x((double)v.x), y((double)v.y), z((double)v.z) {}
inline dvec3::dvec3(const vec3& v):x((double)v.x), y((double)v.y), z((double)v.z) {}
inline dvec4::dvec4(const ivec4& v):x((double)v.x), y((double)v.y), z((double)v.z), w((double)v.w) {}
inline dvec4::dvec4(const uvec4& v):x((double)v.x), y((double)v.y), z((double)v.z), w((double)v.w) {}
inline dvec4::dvec4(const vec4& v):x((double)v.x), y((double)v.y), z((double)v.z), w((double)v.w) {}
inline ivec2::ivec2(const uvec2& v):x((int)v.x), y((int)v.y) {}
inline ivec2::ivec2(const vec2& v):x((int)v.x), y((int)v.y) {}
inline ivec2::ivec2(const dvec2& v):x((int)v.x), y((int)v.y) {}
inline ivec3::ivec3(const uvec3& v):x((int)v.x), y((int)v.y), z((int)v.z) {}
inline ivec3::ivec3(const vec3& v):x((int)v.x), y((int)v.y), z((int)v.z) {}
inline ivec3::ivec3(const dvec3& v):x((int)v.x), y((int)v.y), z((int)v.z) {}
inline ivec4::ivec4(const uvec4& v):x((int)v.x), y((int)v.y), z((int)v.z), w((int)v.w) {}
inline ivec4::ivec4(const vec4& v):x((int)v.x), y((int)v.y), z((int)v.z), w((int)v.w) {}
inline ivec4::ivec4(const dvec4& v):x((int)v.x), y((int)v.y), z((int)v.z), w((int)v.w) {}
inline uvec2::uvec2(const ivec2& v):x((unsigned int)v.x), y((unsigned int)v.y) {}
inline uvec2::uvec2(const vec2& v):x((unsigned int)v.x), y((unsigned int)v.y) {}
inline uvec2::uvec2(const dvec2& v):x((unsigned int)v.x), y((unsigned int)v.y) {}
inline uvec3::uvec3(const ivec3& v):x((unsigned int)v.x), y((unsigned int)v.y), z((unsigned int)v.z) {}
inline uvec3::uvec3(const vec3& v):x((unsigned int)v.x), y((unsigned int)v.y), z((unsigned int)v.z) {}
inline uvec3::uvec3(const dvec3& v):x((unsigned int)v.x), y((unsigned int)v.y), z((unsigned int)v.z) {}
inline uvec4::uvec4(const ivec4& v):x((unsigned int)v.x), y((unsigned int)v.y), z((unsigned int)v.z), w((unsigned int)v.w) {}
inline uvec4::uvec4(const vec4& v):x((unsigned int)v.x), y((unsigned int)v.y), z((unsigned int)v.z), w((unsigned int)v.w) {}
inline uvec4::uvec4(const dvec4& v):x((unsigned int)v.x), y((unsigned int)v.y), z((unsigned int)v.z), w((unsigned int)v.w) {}
inline vec2 operator+(vec2 a, vec2 b) {
	return vec2(a.x+b.x, a.y+b.y);
}
inline vec2 operator-(vec2 a, vec2 b) {
	return vec2(a.x-b.x, a.y-b.y);
}
inline vec2 operator*(vec2 a, vec2 b) {
	return vec2(a.x*b.x, a.y*b.y);
}
inline vec2 operator/(vec2 a, vec2 b) {
	return vec2(a.x/b.x, a.y/b.y);
}
inline vec2 operator-(vec2 v) {
	return vec2(-v.x, -v.y);
}
inline bvec2 operator==(vec2 a, vec2 b) {
	return bvec2(a.x==b.x, a.y==b.y);
}
inline bvec2 operator!=(vec2 a, vec2 b) {
	return bvec2(a.x!=b.x, a.y!=b.y);
}
inline bvec2 operator>(vec2 a, vec2 b) {
	return bvec2(a.x>b.x, a.y>b.y);
}
inline bvec2 operator>=(vec2 a, vec2 b) {
	return bvec2(a.x>=b.x, a.y>=b.y);
}
inline bvec2 operator<(vec2 a, vec2 b) {
	return bvec2(a.x<b.x, a.y<b.y);
}
inline bvec2 operator<=(vec2 a, vec2 b) {
	return bvec2(a.x<=b.x, a.y<=b.y);
}
inline float sum(vec2 v) {
	return v.x+v.y;
}
inline float product(vec2 v) {
	return v.x*v.y;
}
inline void operator+=(vec2& lhs, vec2 rhs) {
	lhs.x+=rhs.x;
	lhs.y+=rhs.y;
}
inline void operator-=(vec2& lhs, vec2 rhs) {
	lhs.x-=rhs.x;
	lhs.y-=rhs.y;
}
inline void operator*=(vec2& lhs, vec2 rhs) {
	lhs.x*=rhs.x;
	lhs.y*=rhs.y;
}
inline void operator/=(vec2& lhs, vec2 rhs) {
	lhs.x/=rhs.x;
	lhs.y/=rhs.y;
}
inline vec3 operator+(vec3 a, vec3 b) {
	return vec3(a.x+b.x, a.y+b.y, a.z+b.z);
}
inline vec3 operator-(vec3 a, vec3 b) {
	return vec3(a.x-b.x, a.y-b.y, a.z-b.z);
}
inline vec3 operator*(vec3 a, vec3 b) {
	return vec3(a.x*b.x, a.y*b.y, a.z*b.z);
}
inline vec3 operator/(vec3 a, vec3 b) {
	return vec3(a.x/b.x, a.y/b.y, a.z/b.z);
}
inline vec3 operator-(vec3 v) {
	return vec3(-v.x, -v.y, -v.z);
}
inline bvec3 operator==(vec3 a, vec3 b) {
	return bvec3(a.x==b.x, a.y==b.y, a.z==b.z);
}
inline bvec3 operator!=(vec3 a, vec3 b) {
	return bvec3(a.x!=b.x, a.y!=b.y, a.z!=b.z);
}
inline bvec3 operator>(vec3 a, vec3 b) {
	return bvec3(a.x>b.x, a.y>b.y, a.z>b.z);
}
inline bvec3 operator>=(vec3 a, vec3 b) {
	return bvec3(a.x>=b.x, a.y>=b.y, a.z>=b.z);
}
inline bvec3 operator<(vec3 a, vec3 b) {
	return bvec3(a.x<b.x, a.y<b.y, a.z<b.z);
}
inline bvec3 operator<=(vec3 a, vec3 b) {
	return bvec3(a.x<=b.x, a.y<=b.y, a.z<=b.z);
}
inline float sum(vec3 v) {
	return v.x+v.y+v.z;
}
inline float product(vec3 v) {
	return v.x*v.y*v.z;
}
inline void operator+=(vec3& lhs, vec3 rhs) {
	lhs.x+=rhs.x;
	lhs.y+=rhs.y;
	lhs.z+=rhs.z;
}
inline void operator-=(vec3& lhs, vec3 rhs) {
	lhs.x-=rhs.x;
	lhs.y-=rhs.y;
	lhs.z-=rhs.z;
}
inline void operator*=(vec3& lhs, vec3 rhs) {
	lhs.x*=rhs.x;
	lhs.y*=rhs.y;
	lhs.z*=rhs.z;
}
inline void operator/=(vec3& lhs, vec3 rhs) {
	lhs.x/=rhs.x;
	lhs.y/=rhs.y;
	lhs.z/=rhs.z;
}
inline vec4 operator+(vec4 a, vec4 b) {
	return vec4(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w);
}
inline vec4 operator-(vec4 a, vec4 b) {
	return vec4(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w);
}
inline vec4 operator*(vec4 a, vec4 b) {
	return vec4(a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w);
}
inline vec4 operator/(vec4 a, vec4 b) {
	return vec4(a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w);
}
inline vec4 operator-(vec4 v) {
	return vec4(-v.x, -v.y, -v.z, -v.w);
}
inline bvec4 operator==(vec4 a, vec4 b) {
	return bvec4(a.x==b.x, a.y==b.y, a.z==b.z, a.w==b.w);
}
inline bvec4 operator!=(vec4 a, vec4 b) {
	return bvec4(a.x!=b.x, a.y!=b.y, a.z!=b.z, a.w!=b.w);
}
inline bvec4 operator>(vec4 a, vec4 b) {
	return bvec4(a.x>b.x, a.y>b.y, a.z>b.z, a.w>b.w);
}
inline bvec4 operator>=(vec4 a, vec4 b) {
	return bvec4(a.x>=b.x, a.y>=b.y, a.z>=b.z, a.w>=b.w);
}
inline bvec4 operator<(vec4 a, vec4 b) {
	return bvec4(a.x<b.x, a.y<b.y, a.z<b.z, a.w<b.w);
}
inline bvec4 operator<=(vec4 a, vec4 b) {
	return bvec4(a.x<=b.x, a.y<=b.y, a.z<=b.z, a.w<=b.w);
}
inline float sum(vec4 v) {
	return v.x+v.y+v.z+v.w;
}
inline float product(vec4 v) {
	return v.x*v.y*v.z*v.w;
}
inline void operator+=(vec4& lhs, vec4 rhs) {
	lhs.x+=rhs.x;
	lhs.y+=rhs.y;
	lhs.z+=rhs.z;
	lhs.w+=rhs.w;
}
inline void operator-=(vec4& lhs, vec4 rhs) {
	lhs.x-=rhs.x;
	lhs.y-=rhs.y;
	lhs.z-=rhs.z;
	lhs.w-=rhs.w;
}
inline void operator*=(vec4& lhs, vec4 rhs) {
	lhs.x*=rhs.x;
	lhs.y*=rhs.y;
	lhs.z*=rhs.z;
	lhs.w*=rhs.w;
}
inline void operator/=(vec4& lhs, vec4 rhs) {
	lhs.x/=rhs.x;
	lhs.y/=rhs.y;
	lhs.z/=rhs.z;
	lhs.w/=rhs.w;
}
inline dvec2 operator+(dvec2 a, dvec2 b) {
	return dvec2(a.x+b.x, a.y+b.y);
}
inline dvec2 operator-(dvec2 a, dvec2 b) {
	return dvec2(a.x-b.x, a.y-b.y);
}
inline dvec2 operator*(dvec2 a, dvec2 b) {
	return dvec2(a.x*b.x, a.y*b.y);
}
inline dvec2 operator/(dvec2 a, dvec2 b) {
	return dvec2(a.x/b.x, a.y/b.y);
}
inline dvec2 operator-(dvec2 v) {
	return dvec2(-v.x, -v.y);
}
inline bvec2 operator==(dvec2 a, dvec2 b) {
	return bvec2(a.x==b.x, a.y==b.y);
}
inline bvec2 operator!=(dvec2 a, dvec2 b) {
	return bvec2(a.x!=b.x, a.y!=b.y);
}
inline bvec2 operator>(dvec2 a, dvec2 b) {
	return bvec2(a.x>b.x, a.y>b.y);
}
inline bvec2 operator>=(dvec2 a, dvec2 b) {
	return bvec2(a.x>=b.x, a.y>=b.y);
}
inline bvec2 operator<(dvec2 a, dvec2 b) {
	return bvec2(a.x<b.x, a.y<b.y);
}
inline bvec2 operator<=(dvec2 a, dvec2 b) {
	return bvec2(a.x<=b.x, a.y<=b.y);
}
inline double sum(dvec2 v) {
	return v.x+v.y;
}
inline double product(dvec2 v) {
	return v.x*v.y;
}
inline void operator+=(dvec2& lhs, dvec2 rhs) {
	lhs.x+=rhs.x;
	lhs.y+=rhs.y;
}
inline void operator-=(dvec2& lhs, dvec2 rhs) {
	lhs.x-=rhs.x;
	lhs.y-=rhs.y;
}
inline void operator*=(dvec2& lhs, dvec2 rhs) {
	lhs.x*=rhs.x;
	lhs.y*=rhs.y;
}
inline void operator/=(dvec2& lhs, dvec2 rhs) {
	lhs.x/=rhs.x;
	lhs.y/=rhs.y;
}
inline dvec3 operator+(dvec3 a, dvec3 b) {
	return dvec3(a.x+b.x, a.y+b.y, a.z+b.z);
}
inline dvec3 operator-(dvec3 a, dvec3 b) {
	return dvec3(a.x-b.x, a.y-b.y, a.z-b.z);
}
inline dvec3 operator*(dvec3 a, dvec3 b) {
	return dvec3(a.x*b.x, a.y*b.y, a.z*b.z);
}
inline dvec3 operator/(dvec3 a, dvec3 b) {
	return dvec3(a.x/b.x, a.y/b.y, a.z/b.z);
}
inline dvec3 operator-(dvec3 v) {
	return dvec3(-v.x, -v.y, -v.z);
}
inline bvec3 operator==(dvec3 a, dvec3 b) {
	return bvec3(a.x==b.x, a.y==b.y, a.z==b.z);
}
inline bvec3 operator!=(dvec3 a, dvec3 b) {
	return bvec3(a.x!=b.x, a.y!=b.y, a.z!=b.z);
}
inline bvec3 operator>(dvec3 a, dvec3 b) {
	return bvec3(a.x>b.x, a.y>b.y, a.z>b.z);
}
inline bvec3 operator>=(dvec3 a, dvec3 b) {
	return bvec3(a.x>=b.x, a.y>=b.y, a.z>=b.z);
}
inline bvec3 operator<(dvec3 a, dvec3 b) {
	return bvec3(a.x<b.x, a.y<b.y, a.z<b.z);
}
inline bvec3 operator<=(dvec3 a, dvec3 b) {
	return bvec3(a.x<=b.x, a.y<=b.y, a.z<=b.z);
}
inline double sum(dvec3 v) {
	return v.x+v.y+v.z;
}
inline double product(dvec3 v) {
	return v.x*v.y*v.z;
}
inline void operator+=(dvec3& lhs, dvec3 rhs) {
	lhs.x+=rhs.x;
	lhs.y+=rhs.y;
	lhs.z+=rhs.z;
}
inline void operator-=(dvec3& lhs, dvec3 rhs) {
	lhs.x-=rhs.x;
	lhs.y-=rhs.y;
	lhs.z-=rhs.z;
}
inline void operator*=(dvec3& lhs, dvec3 rhs) {
	lhs.x*=rhs.x;
	lhs.y*=rhs.y;
	lhs.z*=rhs.z;
}
inline void operator/=(dvec3& lhs, dvec3 rhs) {
	lhs.x/=rhs.x;
	lhs.y/=rhs.y;
	lhs.z/=rhs.z;
}
inline dvec4 operator+(dvec4 a, dvec4 b) {
	return dvec4(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w);
}
inline dvec4 operator-(dvec4 a, dvec4 b) {
	return dvec4(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w);
}
inline dvec4 operator*(dvec4 a, dvec4 b) {
	return dvec4(a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w);
}
inline dvec4 operator/(dvec4 a, dvec4 b) {
	return dvec4(a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w);
}
inline dvec4 operator-(dvec4 v) {
	return dvec4(-v.x, -v.y, -v.z, -v.w);
}
inline bvec4 operator==(dvec4 a, dvec4 b) {
	return bvec4(a.x==b.x, a.y==b.y, a.z==b.z, a.w==b.w);
}
inline bvec4 operator!=(dvec4 a, dvec4 b) {
	return bvec4(a.x!=b.x, a.y!=b.y, a.z!=b.z, a.w!=b.w);
}
inline bvec4 operator>(dvec4 a, dvec4 b) {
	return bvec4(a.x>b.x, a.y>b.y, a.z>b.z, a.w>b.w);
}
inline bvec4 operator>=(dvec4 a, dvec4 b) {
	return bvec4(a.x>=b.x, a.y>=b.y, a.z>=b.z, a.w>=b.w);
}
inline bvec4 operator<(dvec4 a, dvec4 b) {
	return bvec4(a.x<b.x, a.y<b.y, a.z<b.z, a.w<b.w);
}
inline bvec4 operator<=(dvec4 a, dvec4 b) {
	return bvec4(a.x<=b.x, a.y<=b.y, a.z<=b.z, a.w<=b.w);
}
inline double sum(dvec4 v) {
	return v.x+v.y+v.z+v.w;
}
inline double product(dvec4 v) {
	return v.x*v.y*v.z*v.w;
}
inline void operator+=(dvec4& lhs, dvec4 rhs) {
	lhs.x+=rhs.x;
	lhs.y+=rhs.y;
	lhs.z+=rhs.z;
	lhs.w+=rhs.w;
}
inline void operator-=(dvec4& lhs, dvec4 rhs) {
	lhs.x-=rhs.x;
	lhs.y-=rhs.y;
	lhs.z-=rhs.z;
	lhs.w-=rhs.w;
}
inline void operator*=(dvec4& lhs, dvec4 rhs) {
	lhs.x*=rhs.x;
	lhs.y*=rhs.y;
	lhs.z*=rhs.z;
	lhs.w*=rhs.w;
}
inline void operator/=(dvec4& lhs, dvec4 rhs) {
	lhs.x/=rhs.x;
	lhs.y/=rhs.y;
	lhs.z/=rhs.z;
	lhs.w/=rhs.w;
}
inline ivec2 operator+(ivec2 a, ivec2 b) {
	return ivec2(a.x+b.x, a.y+b.y);
}
inline ivec2 operator-(ivec2 a, ivec2 b) {
	return ivec2(a.x-b.x, a.y-b.y);
}
inline ivec2 operator*(ivec2 a, ivec2 b) {
	return ivec2(a.x*b.x, a.y*b.y);
}
inline ivec2 operator/(ivec2 a, ivec2 b) {
	return ivec2(a.x/b.x, a.y/b.y);
}
inline ivec2 operator-(ivec2 v) {
	return ivec2(-v.x, -v.y);
}
inline bvec2 operator==(ivec2 a, ivec2 b) {
	return bvec2(a.x==b.x, a.y==b.y);
}
inline bvec2 operator!=(ivec2 a, ivec2 b) {
	return bvec2(a.x!=b.x, a.y!=b.y);
}
inline bvec2 operator>(ivec2 a, ivec2 b) {
	return bvec2(a.x>b.x, a.y>b.y);
}
inline bvec2 operator>=(ivec2 a, ivec2 b) {
	return bvec2(a.x>=b.x, a.y>=b.y);
}
inline bvec2 operator<(ivec2 a, ivec2 b) {
	return bvec2(a.x<b.x, a.y<b.y);
}
inline bvec2 operator<=(ivec2 a, ivec2 b) {
	return bvec2(a.x<=b.x, a.y<=b.y);
}
inline int sum(ivec2 v) {
	return v.x+v.y;
}
inline int product(ivec2 v) {
	return v.x*v.y;
}
inline void operator+=(ivec2& lhs, ivec2 rhs) {
	lhs.x+=rhs.x;
	lhs.y+=rhs.y;
}
inline void operator-=(ivec2& lhs, ivec2 rhs) {
	lhs.x-=rhs.x;
	lhs.y-=rhs.y;
}
inline void operator*=(ivec2& lhs, ivec2 rhs) {
	lhs.x*=rhs.x;
	lhs.y*=rhs.y;
}
inline void operator/=(ivec2& lhs, ivec2 rhs) {
	lhs.x/=rhs.x;
	lhs.y/=rhs.y;
}
inline ivec3 operator+(ivec3 a, ivec3 b) {
	return ivec3(a.x+b.x, a.y+b.y, a.z+b.z);
}
inline ivec3 operator-(ivec3 a, ivec3 b) {
	return ivec3(a.x-b.x, a.y-b.y, a.z-b.z);
}
inline ivec3 operator*(ivec3 a, ivec3 b) {
	return ivec3(a.x*b.x, a.y*b.y, a.z*b.z);
}
inline ivec3 operator/(ivec3 a, ivec3 b) {
	return ivec3(a.x/b.x, a.y/b.y, a.z/b.z);
}
inline ivec3 operator-(ivec3 v) {
	return ivec3(-v.x, -v.y, -v.z);
}
inline bvec3 operator==(ivec3 a, ivec3 b) {
	return bvec3(a.x==b.x, a.y==b.y, a.z==b.z);
}
inline bvec3 operator!=(ivec3 a, ivec3 b) {
	return bvec3(a.x!=b.x, a.y!=b.y, a.z!=b.z);
}
inline bvec3 operator>(ivec3 a, ivec3 b) {
	return bvec3(a.x>b.x, a.y>b.y, a.z>b.z);
}
inline bvec3 operator>=(ivec3 a, ivec3 b) {
	return bvec3(a.x>=b.x, a.y>=b.y, a.z>=b.z);
}
inline bvec3 operator<(ivec3 a, ivec3 b) {
	return bvec3(a.x<b.x, a.y<b.y, a.z<b.z);
}
inline bvec3 operator<=(ivec3 a, ivec3 b) {
	return bvec3(a.x<=b.x, a.y<=b.y, a.z<=b.z);
}
inline int sum(ivec3 v) {
	return v.x+v.y+v.z;
}
inline int product(ivec3 v) {
	return v.x*v.y*v.z;
}
inline void operator+=(ivec3& lhs, ivec3 rhs) {
	lhs.x+=rhs.x;
	lhs.y+=rhs.y;
	lhs.z+=rhs.z;
}
inline void operator-=(ivec3& lhs, ivec3 rhs) {
	lhs.x-=rhs.x;
	lhs.y-=rhs.y;
	lhs.z-=rhs.z;
}
inline void operator*=(ivec3& lhs, ivec3 rhs) {
	lhs.x*=rhs.x;
	lhs.y*=rhs.y;
	lhs.z*=rhs.z;
}
inline void operator/=(ivec3& lhs, ivec3 rhs) {
	lhs.x/=rhs.x;
	lhs.y/=rhs.y;
	lhs.z/=rhs.z;
}
inline ivec4 operator+(ivec4 a, ivec4 b) {
	return ivec4(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w);
}
inline ivec4 operator-(ivec4 a, ivec4 b) {
	return ivec4(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w);
}
inline ivec4 operator*(ivec4 a, ivec4 b) {
	return ivec4(a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w);
}
inline ivec4 operator/(ivec4 a, ivec4 b) {
	return ivec4(a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w);
}
inline ivec4 operator-(ivec4 v) {
	return ivec4(-v.x, -v.y, -v.z, -v.w);
}
inline bvec4 operator==(ivec4 a, ivec4 b) {
	return bvec4(a.x==b.x, a.y==b.y, a.z==b.z, a.w==b.w);
}
inline bvec4 operator!=(ivec4 a, ivec4 b) {
	return bvec4(a.x!=b.x, a.y!=b.y, a.z!=b.z, a.w!=b.w);
}
inline bvec4 operator>(ivec4 a, ivec4 b) {
	return bvec4(a.x>b.x, a.y>b.y, a.z>b.z, a.w>b.w);
}
inline bvec4 operator>=(ivec4 a, ivec4 b) {
	return bvec4(a.x>=b.x, a.y>=b.y, a.z>=b.z, a.w>=b.w);
}
inline bvec4 operator<(ivec4 a, ivec4 b) {
	return bvec4(a.x<b.x, a.y<b.y, a.z<b.z, a.w<b.w);
}
inline bvec4 operator<=(ivec4 a, ivec4 b) {
	return bvec4(a.x<=b.x, a.y<=b.y, a.z<=b.z, a.w<=b.w);
}
inline int sum(ivec4 v) {
	return v.x+v.y+v.z+v.w;
}
inline int product(ivec4 v) {
	return v.x*v.y*v.z*v.w;
}
inline void operator+=(ivec4& lhs, ivec4 rhs) {
	lhs.x+=rhs.x;
	lhs.y+=rhs.y;
	lhs.z+=rhs.z;
	lhs.w+=rhs.w;
}
inline void operator-=(ivec4& lhs, ivec4 rhs) {
	lhs.x-=rhs.x;
	lhs.y-=rhs.y;
	lhs.z-=rhs.z;
	lhs.w-=rhs.w;
}
inline void operator*=(ivec4& lhs, ivec4 rhs) {
	lhs.x*=rhs.x;
	lhs.y*=rhs.y;
	lhs.z*=rhs.z;
	lhs.w*=rhs.w;
}
inline void operator/=(ivec4& lhs, ivec4 rhs) {
	lhs.x/=rhs.x;
	lhs.y/=rhs.y;
	lhs.z/=rhs.z;
	lhs.w/=rhs.w;
}
inline uvec2 operator+(uvec2 a, uvec2 b) {
	return uvec2(a.x+b.x, a.y+b.y);
}
inline uvec2 operator-(uvec2 a, uvec2 b) {
	return uvec2(a.x-b.x, a.y-b.y);
}
inline uvec2 operator*(uvec2 a, uvec2 b) {
	return uvec2(a.x*b.x, a.y*b.y);
}
inline uvec2 operator/(uvec2 a, uvec2 b) {
	return uvec2(a.x/b.x, a.y/b.y);
}
inline uvec2 operator-(uvec2 v) {
	return uvec2(-v.x, -v.y);
}
inline bvec2 operator==(uvec2 a, uvec2 b) {
	return bvec2(a.x==b.x, a.y==b.y);
}
inline bvec2 operator!=(uvec2 a, uvec2 b) {
	return bvec2(a.x!=b.x, a.y!=b.y);
}
inline bvec2 operator>(uvec2 a, uvec2 b) {
	return bvec2(a.x>b.x, a.y>b.y);
}
inline bvec2 operator>=(uvec2 a, uvec2 b) {
	return bvec2(a.x>=b.x, a.y>=b.y);
}
inline bvec2 operator<(uvec2 a, uvec2 b) {
	return bvec2(a.x<b.x, a.y<b.y);
}
inline bvec2 operator<=(uvec2 a, uvec2 b) {
	return bvec2(a.x<=b.x, a.y<=b.y);
}
inline unsigned int sum(uvec2 v) {
	return v.x+v.y;
}
inline unsigned int product(uvec2 v) {
	return v.x*v.y;
}
inline void operator+=(uvec2& lhs, uvec2 rhs) {
	lhs.x+=rhs.x;
	lhs.y+=rhs.y;
}
inline void operator-=(uvec2& lhs, uvec2 rhs) {
	lhs.x-=rhs.x;
	lhs.y-=rhs.y;
}
inline void operator*=(uvec2& lhs, uvec2 rhs) {
	lhs.x*=rhs.x;
	lhs.y*=rhs.y;
}
inline void operator/=(uvec2& lhs, uvec2 rhs) {
	lhs.x/=rhs.x;
	lhs.y/=rhs.y;
}
inline uvec3 operator+(uvec3 a, uvec3 b) {
	return uvec3(a.x+b.x, a.y+b.y, a.z+b.z);
}
inline uvec3 operator-(uvec3 a, uvec3 b) {
	return uvec3(a.x-b.x, a.y-b.y, a.z-b.z);
}
inline uvec3 operator*(uvec3 a, uvec3 b) {
	return uvec3(a.x*b.x, a.y*b.y, a.z*b.z);
}
inline uvec3 operator/(uvec3 a, uvec3 b) {
	return uvec3(a.x/b.x, a.y/b.y, a.z/b.z);
}
inline uvec3 operator-(uvec3 v) {
	return uvec3(-v.x, -v.y, -v.z);
}
inline bvec3 operator==(uvec3 a, uvec3 b) {
	return bvec3(a.x==b.x, a.y==b.y, a.z==b.z);
}
inline bvec3 operator!=(uvec3 a, uvec3 b) {
	return bvec3(a.x!=b.x, a.y!=b.y, a.z!=b.z);
}
inline bvec3 operator>(uvec3 a, uvec3 b) {
	return bvec3(a.x>b.x, a.y>b.y, a.z>b.z);
}
inline bvec3 operator>=(uvec3 a, uvec3 b) {
	return bvec3(a.x>=b.x, a.y>=b.y, a.z>=b.z);
}
inline bvec3 operator<(uvec3 a, uvec3 b) {
	return bvec3(a.x<b.x, a.y<b.y, a.z<b.z);
}
inline bvec3 operator<=(uvec3 a, uvec3 b) {
	return bvec3(a.x<=b.x, a.y<=b.y, a.z<=b.z);
}
inline unsigned int sum(uvec3 v) {
	return v.x+v.y+v.z;
}
inline unsigned int product(uvec3 v) {
	return v.x*v.y*v.z;
}
inline void operator+=(uvec3& lhs, uvec3 rhs) {
	lhs.x+=rhs.x;
	lhs.y+=rhs.y;
	lhs.z+=rhs.z;
}
inline void operator-=(uvec3& lhs, uvec3 rhs) {
	lhs.x-=rhs.x;
	lhs.y-=rhs.y;
	lhs.z-=rhs.z;
}
inline void operator*=(uvec3& lhs, uvec3 rhs) {
	lhs.x*=rhs.x;
	lhs.y*=rhs.y;
	lhs.z*=rhs.z;
}
inline void operator/=(uvec3& lhs, uvec3 rhs) {
	lhs.x/=rhs.x;
	lhs.y/=rhs.y;
	lhs.z/=rhs.z;
}
inline uvec4 operator+(uvec4 a, uvec4 b) {
	return uvec4(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w);
}
inline uvec4 operator-(uvec4 a, uvec4 b) {
	return uvec4(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w);
}
inline uvec4 operator*(uvec4 a, uvec4 b) {
	return uvec4(a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w);
}
inline uvec4 operator/(uvec4 a, uvec4 b) {
	return uvec4(a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w);
}
inline uvec4 operator-(uvec4 v) {
	return uvec4(-v.x, -v.y, -v.z, -v.w);
}
inline bvec4 operator==(uvec4 a, uvec4 b) {
	return bvec4(a.x==b.x, a.y==b.y, a.z==b.z, a.w==b.w);
}
inline bvec4 operator!=(uvec4 a, uvec4 b) {
	return bvec4(a.x!=b.x, a.y!=b.y, a.z!=b.z, a.w!=b.w);
}
inline bvec4 operator>(uvec4 a, uvec4 b) {
	return bvec4(a.x>b.x, a.y>b.y, a.z>b.z, a.w>b.w);
}
inline bvec4 operator>=(uvec4 a, uvec4 b) {
	return bvec4(a.x>=b.x, a.y>=b.y, a.z>=b.z, a.w>=b.w);
}
inline bvec4 operator<(uvec4 a, uvec4 b) {
	return bvec4(a.x<b.x, a.y<b.y, a.z<b.z, a.w<b.w);
}
inline bvec4 operator<=(uvec4 a, uvec4 b) {
	return bvec4(a.x<=b.x, a.y<=b.y, a.z<=b.z, a.w<=b.w);
}
inline unsigned int sum(uvec4 v) {
	return v.x+v.y+v.z+v.w;
}
inline unsigned int product(uvec4 v) {
	return v.x*v.y*v.z*v.w;
}
inline void operator+=(uvec4& lhs, uvec4 rhs) {
	lhs.x+=rhs.x;
	lhs.y+=rhs.y;
	lhs.z+=rhs.z;
	lhs.w+=rhs.w;
}
inline void operator-=(uvec4& lhs, uvec4 rhs) {
	lhs.x-=rhs.x;
	lhs.y-=rhs.y;
	lhs.z-=rhs.z;
	lhs.w-=rhs.w;
}
inline void operator*=(uvec4& lhs, uvec4 rhs) {
	lhs.x*=rhs.x;
	lhs.y*=rhs.y;
	lhs.z*=rhs.z;
	lhs.w*=rhs.w;
}
inline void operator/=(uvec4& lhs, uvec4 rhs) {
	lhs.x/=rhs.x;
	lhs.y/=rhs.y;
	lhs.z/=rhs.z;
	lhs.w/=rhs.w;
}
inline float mincomp(vec2 v) {
	return min(v.x, v.y);
}
inline float maxcomp(vec2 v) {
	return max(v.x, v.y);
}
inline vec2 sq(vec2 a) {
	return vec2(sq(a.x), sq(a.y));
}
inline vec2 abs(vec2 a) {
	return vec2(abs(a.x), abs(a.y));
}
inline vec2 min(vec2 a, vec2 b) {
	return vec2(min(a.x, b.x), min(a.y, b.y));
}
inline vec2 max(vec2 a, vec2 b) {
	return vec2(max(a.x, b.x), max(a.y, b.y));
}
inline vec2 clamp(vec2 a, vec2 b, vec2 c) {
	return vec2(clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y));
}
inline float mincomp(vec3 v) {
	return min(min(v.x, v.y), v.z);
}
inline float maxcomp(vec3 v) {
	return max(max(v.x, v.y), v.z);
}
inline vec3 sq(vec3 a) {
	return vec3(sq(a.x), sq(a.y), sq(a.z));
}
inline vec3 abs(vec3 a) {
	return vec3(abs(a.x), abs(a.y), abs(a.z));
}
inline vec3 min(vec3 a, vec3 b) {
	return vec3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}
inline vec3 max(vec3 a, vec3 b) {
	return vec3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}
inline vec3 clamp(vec3 a, vec3 b, vec3 c) {
	return vec3(clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y), clamp(a.z, b.z, c.z));
}
inline float mincomp(vec4 v) {
	return min(min(min(v.x, v.y), v.z), v.w);
}
inline float maxcomp(vec4 v) {
	return max(max(max(v.x, v.y), v.z), v.w);
}
inline vec4 sq(vec4 a) {
	return vec4(sq(a.x), sq(a.y), sq(a.z), sq(a.w));
}
inline vec4 abs(vec4 a) {
	return vec4(abs(a.x), abs(a.y), abs(a.z), abs(a.w));
}
inline vec4 min(vec4 a, vec4 b) {
	return vec4(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w));
}
inline vec4 max(vec4 a, vec4 b) {
	return vec4(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w));
}
inline vec4 clamp(vec4 a, vec4 b, vec4 c) {
	return vec4(clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y), clamp(a.z, b.z, c.z), clamp(a.w, b.w, c.w));
}
inline double mincomp(dvec2 v) {
	return min(v.x, v.y);
}
inline double maxcomp(dvec2 v) {
	return max(v.x, v.y);
}
inline dvec2 sq(dvec2 a) {
	return dvec2(sq(a.x), sq(a.y));
}
inline dvec2 abs(dvec2 a) {
	return dvec2(abs(a.x), abs(a.y));
}
inline dvec2 min(dvec2 a, dvec2 b) {
	return dvec2(min(a.x, b.x), min(a.y, b.y));
}
inline dvec2 max(dvec2 a, dvec2 b) {
	return dvec2(max(a.x, b.x), max(a.y, b.y));
}
inline dvec2 clamp(dvec2 a, dvec2 b, dvec2 c) {
	return dvec2(clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y));
}
inline double mincomp(dvec3 v) {
	return min(min(v.x, v.y), v.z);
}
inline double maxcomp(dvec3 v) {
	return max(max(v.x, v.y), v.z);
}
inline dvec3 sq(dvec3 a) {
	return dvec3(sq(a.x), sq(a.y), sq(a.z));
}
inline dvec3 abs(dvec3 a) {
	return dvec3(abs(a.x), abs(a.y), abs(a.z));
}
inline dvec3 min(dvec3 a, dvec3 b) {
	return dvec3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}
inline dvec3 max(dvec3 a, dvec3 b) {
	return dvec3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}
inline dvec3 clamp(dvec3 a, dvec3 b, dvec3 c) {
	return dvec3(clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y), clamp(a.z, b.z, c.z));
}
inline double mincomp(dvec4 v) {
	return min(min(min(v.x, v.y), v.z), v.w);
}
inline double maxcomp(dvec4 v) {
	return max(max(max(v.x, v.y), v.z), v.w);
}
inline dvec4 sq(dvec4 a) {
	return dvec4(sq(a.x), sq(a.y), sq(a.z), sq(a.w));
}
inline dvec4 abs(dvec4 a) {
	return dvec4(abs(a.x), abs(a.y), abs(a.z), abs(a.w));
}
inline dvec4 min(dvec4 a, dvec4 b) {
	return dvec4(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w));
}
inline dvec4 max(dvec4 a, dvec4 b) {
	return dvec4(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w));
}
inline dvec4 clamp(dvec4 a, dvec4 b, dvec4 c) {
	return dvec4(clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y), clamp(a.z, b.z, c.z), clamp(a.w, b.w, c.w));
}
inline int mincomp(ivec2 v) {
	return min(v.x, v.y);
}
inline int maxcomp(ivec2 v) {
	return max(v.x, v.y);
}
inline ivec2 sq(ivec2 a) {
	return ivec2(sq(a.x), sq(a.y));
}
inline ivec2 abs(ivec2 a) {
	return ivec2(abs(a.x), abs(a.y));
}
inline ivec2 min(ivec2 a, ivec2 b) {
	return ivec2(min(a.x, b.x), min(a.y, b.y));
}
inline ivec2 max(ivec2 a, ivec2 b) {
	return ivec2(max(a.x, b.x), max(a.y, b.y));
}
inline ivec2 clamp(ivec2 a, ivec2 b, ivec2 c) {
	return ivec2(clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y));
}
inline int mincomp(ivec3 v) {
	return min(min(v.x, v.y), v.z);
}
inline int maxcomp(ivec3 v) {
	return max(max(v.x, v.y), v.z);
}
inline ivec3 sq(ivec3 a) {
	return ivec3(sq(a.x), sq(a.y), sq(a.z));
}
inline ivec3 abs(ivec3 a) {
	return ivec3(abs(a.x), abs(a.y), abs(a.z));
}
inline ivec3 min(ivec3 a, ivec3 b) {
	return ivec3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}
inline ivec3 max(ivec3 a, ivec3 b) {
	return ivec3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}
inline ivec3 clamp(ivec3 a, ivec3 b, ivec3 c) {
	return ivec3(clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y), clamp(a.z, b.z, c.z));
}
inline int mincomp(ivec4 v) {
	return min(min(min(v.x, v.y), v.z), v.w);
}
inline int maxcomp(ivec4 v) {
	return max(max(max(v.x, v.y), v.z), v.w);
}
inline ivec4 sq(ivec4 a) {
	return ivec4(sq(a.x), sq(a.y), sq(a.z), sq(a.w));
}
inline ivec4 abs(ivec4 a) {
	return ivec4(abs(a.x), abs(a.y), abs(a.z), abs(a.w));
}
inline ivec4 min(ivec4 a, ivec4 b) {
	return ivec4(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w));
}
inline ivec4 max(ivec4 a, ivec4 b) {
	return ivec4(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w));
}
inline ivec4 clamp(ivec4 a, ivec4 b, ivec4 c) {
	return ivec4(clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y), clamp(a.z, b.z, c.z), clamp(a.w, b.w, c.w));
}
inline unsigned int mincomp(uvec2 v) {
	return min(v.x, v.y);
}
inline unsigned int maxcomp(uvec2 v) {
	return max(v.x, v.y);
}
inline uvec2 sq(uvec2 a) {
	return uvec2(sq(a.x), sq(a.y));
}
inline uvec2 abs(uvec2 a) {
	return uvec2(abs(a.x), abs(a.y));
}
inline uvec2 min(uvec2 a, uvec2 b) {
	return uvec2(min(a.x, b.x), min(a.y, b.y));
}
inline uvec2 max(uvec2 a, uvec2 b) {
	return uvec2(max(a.x, b.x), max(a.y, b.y));
}
inline uvec2 clamp(uvec2 a, uvec2 b, uvec2 c) {
	return uvec2(clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y));
}
inline unsigned int mincomp(uvec3 v) {
	return min(min(v.x, v.y), v.z);
}
inline unsigned int maxcomp(uvec3 v) {
	return max(max(v.x, v.y), v.z);
}
inline uvec3 sq(uvec3 a) {
	return uvec3(sq(a.x), sq(a.y), sq(a.z));
}
inline uvec3 abs(uvec3 a) {
	return uvec3(abs(a.x), abs(a.y), abs(a.z));
}
inline uvec3 min(uvec3 a, uvec3 b) {
	return uvec3(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}
inline uvec3 max(uvec3 a, uvec3 b) {
	return uvec3(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}
inline uvec3 clamp(uvec3 a, uvec3 b, uvec3 c) {
	return uvec3(clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y), clamp(a.z, b.z, c.z));
}
inline unsigned int mincomp(uvec4 v) {
	return min(min(min(v.x, v.y), v.z), v.w);
}
inline unsigned int maxcomp(uvec4 v) {
	return max(max(max(v.x, v.y), v.z), v.w);
}
inline uvec4 sq(uvec4 a) {
	return uvec4(sq(a.x), sq(a.y), sq(a.z), sq(a.w));
}
inline uvec4 abs(uvec4 a) {
	return uvec4(abs(a.x), abs(a.y), abs(a.z), abs(a.w));
}
inline uvec4 min(uvec4 a, uvec4 b) {
	return uvec4(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z), min(a.w, b.w));
}
inline uvec4 max(uvec4 a, uvec4 b) {
	return uvec4(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z), max(a.w, b.w));
}
inline uvec4 clamp(uvec4 a, uvec4 b, uvec4 c) {
	return uvec4(clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y), clamp(a.z, b.z, c.z), clamp(a.w, b.w, c.w));
}
inline vec2 sign(vec2 a) {
	return vec2(sign(a.x), sign(a.y));
}
inline vec2 fract(vec2 a) {
	return vec2(fract(a.x), fract(a.y));
}
inline vec2 lerp(vec2 a, vec2 b, vec2 c) {
	return vec2(lerp(a.x, b.x, c.x), lerp(a.y, b.y, c.y));
}
inline float dot(vec2 a, vec2 b) {
	return sum(a*b);
}
inline float lengthsq(vec2 a) {
	return dot(a, a);
}
inline float length(vec2 a) {
	return sqrt(lengthsq(a));
}
inline vec2 normalize(vec2 a) {
	return a/length(a);
}
inline vec3 sign(vec3 a) {
	return vec3(sign(a.x), sign(a.y), sign(a.z));
}
inline vec3 fract(vec3 a) {
	return vec3(fract(a.x), fract(a.y), fract(a.z));
}
inline vec3 lerp(vec3 a, vec3 b, vec3 c) {
	return vec3(lerp(a.x, b.x, c.x), lerp(a.y, b.y, c.y), lerp(a.z, b.z, c.z));
}
inline float dot(vec3 a, vec3 b) {
	return sum(a*b);
}
inline float lengthsq(vec3 a) {
	return dot(a, a);
}
inline float length(vec3 a) {
	return sqrt(lengthsq(a));
}
inline vec3 normalize(vec3 a) {
	return a/length(a);
}
inline vec4 sign(vec4 a) {
	return vec4(sign(a.x), sign(a.y), sign(a.z), sign(a.w));
}
inline vec4 fract(vec4 a) {
	return vec4(fract(a.x), fract(a.y), fract(a.z), fract(a.w));
}
inline vec4 lerp(vec4 a, vec4 b, vec4 c) {
	return vec4(lerp(a.x, b.x, c.x), lerp(a.y, b.y, c.y), lerp(a.z, b.z, c.z), lerp(a.w, b.w, c.w));
}
inline float dot(vec4 a, vec4 b) {
	return sum(a*b);
}
inline float lengthsq(vec4 a) {
	return dot(a, a);
}
inline float length(vec4 a) {
	return sqrt(lengthsq(a));
}
inline vec4 normalize(vec4 a) {
	return a/length(a);
}
inline dvec2 sign(dvec2 a) {
	return dvec2(sign(a.x), sign(a.y));
}
inline dvec2 fract(dvec2 a) {
	return dvec2(fract(a.x), fract(a.y));
}
inline dvec2 lerp(dvec2 a, dvec2 b, dvec2 c) {
	return dvec2(lerp(a.x, b.x, c.x), lerp(a.y, b.y, c.y));
}
inline double dot(dvec2 a, dvec2 b) {
	return sum(a*b);
}
inline double lengthsq(dvec2 a) {
	return dot(a, a);
}
inline double length(dvec2 a) {
	return sqrt(lengthsq(a));
}
inline dvec2 normalize(dvec2 a) {
	return a/length(a);
}
inline dvec3 sign(dvec3 a) {
	return dvec3(sign(a.x), sign(a.y), sign(a.z));
}
inline dvec3 fract(dvec3 a) {
	return dvec3(fract(a.x), fract(a.y), fract(a.z));
}
inline dvec3 lerp(dvec3 a, dvec3 b, dvec3 c) {
	return dvec3(lerp(a.x, b.x, c.x), lerp(a.y, b.y, c.y), lerp(a.z, b.z, c.z));
}
inline double dot(dvec3 a, dvec3 b) {
	return sum(a*b);
}
inline double lengthsq(dvec3 a) {
	return dot(a, a);
}
inline double length(dvec3 a) {
	return sqrt(lengthsq(a));
}
inline dvec3 normalize(dvec3 a) {
	return a/length(a);
}
inline dvec4 sign(dvec4 a) {
	return dvec4(sign(a.x), sign(a.y), sign(a.z), sign(a.w));
}
inline dvec4 fract(dvec4 a) {
	return dvec4(fract(a.x), fract(a.y), fract(a.z), fract(a.w));
}
inline dvec4 lerp(dvec4 a, dvec4 b, dvec4 c) {
	return dvec4(lerp(a.x, b.x, c.x), lerp(a.y, b.y, c.y), lerp(a.z, b.z, c.z), lerp(a.w, b.w, c.w));
}
inline double dot(dvec4 a, dvec4 b) {
	return sum(a*b);
}
inline double lengthsq(dvec4 a) {
	return dot(a, a);
}
inline double length(dvec4 a) {
	return sqrt(lengthsq(a));
}
inline dvec4 normalize(dvec4 a) {
	return a/length(a);
}
inline ivec2 operator&(ivec2 a, ivec2 b) {
	return ivec2(a.x&b.x, a.y&b.y);
}
inline ivec2 operator|(ivec2 a, ivec2 b) {
	return ivec2(a.x|b.x, a.y|b.y);
}
inline ivec2 operator^(ivec2 a, ivec2 b) {
	return ivec2(a.x^b.x, a.y^b.y);
}
inline ivec2 operator<<(ivec2 a, ivec2 b) {
	return ivec2(a.x<<b.x, a.y<<b.y);
}
inline ivec2 operator>>(ivec2 a, ivec2 b) {
	return ivec2(a.x>>b.x, a.y>>b.y);
}
inline ivec2 operator~(ivec2 v) {
	return ivec2(~v.x, ~v.y);
}
inline void operator&=(ivec2& lhs, ivec2 rhs) {
	lhs.x&=rhs.x;
	lhs.y&=rhs.y;
}
inline void operator|=(ivec2& lhs, ivec2 rhs) {
	lhs.x|=rhs.x;
	lhs.y|=rhs.y;
}
inline void operator^=(ivec2& lhs, ivec2 rhs) {
	lhs.x^=rhs.x;
	lhs.y^=rhs.y;
}
inline void operator<<=(ivec2& lhs, ivec2 rhs) {
	lhs.x<<=rhs.x;
	lhs.y<<=rhs.y;
}
inline void operator>>=(ivec2& lhs, ivec2 rhs) {
	lhs.x>>=rhs.x;
	lhs.y>>=rhs.y;
}
inline ivec3 operator&(ivec3 a, ivec3 b) {
	return ivec3(a.x&b.x, a.y&b.y, a.z&b.z);
}
inline ivec3 operator|(ivec3 a, ivec3 b) {
	return ivec3(a.x|b.x, a.y|b.y, a.z|b.z);
}
inline ivec3 operator^(ivec3 a, ivec3 b) {
	return ivec3(a.x^b.x, a.y^b.y, a.z^b.z);
}
inline ivec3 operator<<(ivec3 a, ivec3 b) {
	return ivec3(a.x<<b.x, a.y<<b.y, a.z<<b.z);
}
inline ivec3 operator>>(ivec3 a, ivec3 b) {
	return ivec3(a.x>>b.x, a.y>>b.y, a.z>>b.z);
}
inline ivec3 operator~(ivec3 v) {
	return ivec3(~v.x, ~v.y, ~v.z);
}
inline void operator&=(ivec3& lhs, ivec3 rhs) {
	lhs.x&=rhs.x;
	lhs.y&=rhs.y;
	lhs.z&=rhs.z;
}
inline void operator|=(ivec3& lhs, ivec3 rhs) {
	lhs.x|=rhs.x;
	lhs.y|=rhs.y;
	lhs.z|=rhs.z;
}
inline void operator^=(ivec3& lhs, ivec3 rhs) {
	lhs.x^=rhs.x;
	lhs.y^=rhs.y;
	lhs.z^=rhs.z;
}
inline void operator<<=(ivec3& lhs, ivec3 rhs) {
	lhs.x<<=rhs.x;
	lhs.y<<=rhs.y;
	lhs.z<<=rhs.z;
}
inline void operator>>=(ivec3& lhs, ivec3 rhs) {
	lhs.x>>=rhs.x;
	lhs.y>>=rhs.y;
	lhs.z>>=rhs.z;
}
inline ivec4 operator&(ivec4 a, ivec4 b) {
	return ivec4(a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w);
}
inline ivec4 operator|(ivec4 a, ivec4 b) {
	return ivec4(a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w);
}
inline ivec4 operator^(ivec4 a, ivec4 b) {
	return ivec4(a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w);
}
inline ivec4 operator<<(ivec4 a, ivec4 b) {
	return ivec4(a.x<<b.x, a.y<<b.y, a.z<<b.z, a.w<<b.w);
}
inline ivec4 operator>>(ivec4 a, ivec4 b) {
	return ivec4(a.x>>b.x, a.y>>b.y, a.z>>b.z, a.w>>b.w);
}
inline ivec4 operator~(ivec4 v) {
	return ivec4(~v.x, ~v.y, ~v.z, ~v.w);
}
inline void operator&=(ivec4& lhs, ivec4 rhs) {
	lhs.x&=rhs.x;
	lhs.y&=rhs.y;
	lhs.z&=rhs.z;
	lhs.w&=rhs.w;
}
inline void operator|=(ivec4& lhs, ivec4 rhs) {
	lhs.x|=rhs.x;
	lhs.y|=rhs.y;
	lhs.z|=rhs.z;
	lhs.w|=rhs.w;
}
inline void operator^=(ivec4& lhs, ivec4 rhs) {
	lhs.x^=rhs.x;
	lhs.y^=rhs.y;
	lhs.z^=rhs.z;
	lhs.w^=rhs.w;
}
inline void operator<<=(ivec4& lhs, ivec4 rhs) {
	lhs.x<<=rhs.x;
	lhs.y<<=rhs.y;
	lhs.z<<=rhs.z;
	lhs.w<<=rhs.w;
}
inline void operator>>=(ivec4& lhs, ivec4 rhs) {
	lhs.x>>=rhs.x;
	lhs.y>>=rhs.y;
	lhs.z>>=rhs.z;
	lhs.w>>=rhs.w;
}
inline uvec2 operator&(uvec2 a, uvec2 b) {
	return uvec2(a.x&b.x, a.y&b.y);
}
inline uvec2 operator|(uvec2 a, uvec2 b) {
	return uvec2(a.x|b.x, a.y|b.y);
}
inline uvec2 operator^(uvec2 a, uvec2 b) {
	return uvec2(a.x^b.x, a.y^b.y);
}
inline uvec2 operator<<(uvec2 a, uvec2 b) {
	return uvec2(a.x<<b.x, a.y<<b.y);
}
inline uvec2 operator>>(uvec2 a, uvec2 b) {
	return uvec2(a.x>>b.x, a.y>>b.y);
}
inline uvec2 operator~(uvec2 v) {
	return uvec2(~v.x, ~v.y);
}
inline void operator&=(uvec2& lhs, uvec2 rhs) {
	lhs.x&=rhs.x;
	lhs.y&=rhs.y;
}
inline void operator|=(uvec2& lhs, uvec2 rhs) {
	lhs.x|=rhs.x;
	lhs.y|=rhs.y;
}
inline void operator^=(uvec2& lhs, uvec2 rhs) {
	lhs.x^=rhs.x;
	lhs.y^=rhs.y;
}
inline void operator<<=(uvec2& lhs, uvec2 rhs) {
	lhs.x<<=rhs.x;
	lhs.y<<=rhs.y;
}
inline void operator>>=(uvec2& lhs, uvec2 rhs) {
	lhs.x>>=rhs.x;
	lhs.y>>=rhs.y;
}
inline uvec3 operator&(uvec3 a, uvec3 b) {
	return uvec3(a.x&b.x, a.y&b.y, a.z&b.z);
}
inline uvec3 operator|(uvec3 a, uvec3 b) {
	return uvec3(a.x|b.x, a.y|b.y, a.z|b.z);
}
inline uvec3 operator^(uvec3 a, uvec3 b) {
	return uvec3(a.x^b.x, a.y^b.y, a.z^b.z);
}
inline uvec3 operator<<(uvec3 a, uvec3 b) {
	return uvec3(a.x<<b.x, a.y<<b.y, a.z<<b.z);
}
inline uvec3 operator>>(uvec3 a, uvec3 b) {
	return uvec3(a.x>>b.x, a.y>>b.y, a.z>>b.z);
}
inline uvec3 operator~(uvec3 v) {
	return uvec3(~v.x, ~v.y, ~v.z);
}
inline void operator&=(uvec3& lhs, uvec3 rhs) {
	lhs.x&=rhs.x;
	lhs.y&=rhs.y;
	lhs.z&=rhs.z;
}
inline void operator|=(uvec3& lhs, uvec3 rhs) {
	lhs.x|=rhs.x;
	lhs.y|=rhs.y;
	lhs.z|=rhs.z;
}
inline void operator^=(uvec3& lhs, uvec3 rhs) {
	lhs.x^=rhs.x;
	lhs.y^=rhs.y;
	lhs.z^=rhs.z;
}
inline void operator<<=(uvec3& lhs, uvec3 rhs) {
	lhs.x<<=rhs.x;
	lhs.y<<=rhs.y;
	lhs.z<<=rhs.z;
}
inline void operator>>=(uvec3& lhs, uvec3 rhs) {
	lhs.x>>=rhs.x;
	lhs.y>>=rhs.y;
	lhs.z>>=rhs.z;
}
inline uvec4 operator&(uvec4 a, uvec4 b) {
	return uvec4(a.x&b.x, a.y&b.y, a.z&b.z, a.w&b.w);
}
inline uvec4 operator|(uvec4 a, uvec4 b) {
	return uvec4(a.x|b.x, a.y|b.y, a.z|b.z, a.w|b.w);
}
inline uvec4 operator^(uvec4 a, uvec4 b) {
	return uvec4(a.x^b.x, a.y^b.y, a.z^b.z, a.w^b.w);
}
inline uvec4 operator<<(uvec4 a, uvec4 b) {
	return uvec4(a.x<<b.x, a.y<<b.y, a.z<<b.z, a.w<<b.w);
}
inline uvec4 operator>>(uvec4 a, uvec4 b) {
	return uvec4(a.x>>b.x, a.y>>b.y, a.z>>b.z, a.w>>b.w);
}
inline uvec4 operator~(uvec4 v) {
	return uvec4(~v.x, ~v.y, ~v.z, ~v.w);
}
inline void operator&=(uvec4& lhs, uvec4 rhs) {
	lhs.x&=rhs.x;
	lhs.y&=rhs.y;
	lhs.z&=rhs.z;
	lhs.w&=rhs.w;
}
inline void operator|=(uvec4& lhs, uvec4 rhs) {
	lhs.x|=rhs.x;
	lhs.y|=rhs.y;
	lhs.z|=rhs.z;
	lhs.w|=rhs.w;
}
inline void operator^=(uvec4& lhs, uvec4 rhs) {
	lhs.x^=rhs.x;
	lhs.y^=rhs.y;
	lhs.z^=rhs.z;
	lhs.w^=rhs.w;
}
inline void operator<<=(uvec4& lhs, uvec4 rhs) {
	lhs.x<<=rhs.x;
	lhs.y<<=rhs.y;
	lhs.z<<=rhs.z;
	lhs.w<<=rhs.w;
}
inline void operator>>=(uvec4& lhs, uvec4 rhs) {
	lhs.x>>=rhs.x;
	lhs.y>>=rhs.y;
	lhs.z>>=rhs.z;
	lhs.w>>=rhs.w;
}
inline bvec2 operator==(bvec2 a, bvec2 b) {
	return bvec2(a.x==b.x, a.y==b.y);
}
inline bvec2 operator!=(bvec2 a, bvec2 b) {
	return bvec2(a.x!=b.x, a.y!=b.y);
}
inline bvec2 operator&&(bvec2 a, bvec2 b) {
	return bvec2(a.x&&b.x, a.y&&b.y);
}
inline bvec2 operator||(bvec2 a, bvec2 b) {
	return bvec2(a.x||b.x, a.y||b.y);
}
inline bvec2 operator!(bvec2 v) {
	return bvec2(!v.x, !v.y);
}
inline bool all(bvec2 v) {
	return v.x&&v.y;
}
inline bool any(bvec2 v) {
	return v.x||v.y;
}
inline bvec3 operator==(bvec3 a, bvec3 b) {
	return bvec3(a.x==b.x, a.y==b.y, a.z==b.z);
}
inline bvec3 operator!=(bvec3 a, bvec3 b) {
	return bvec3(a.x!=b.x, a.y!=b.y, a.z!=b.z);
}
inline bvec3 operator&&(bvec3 a, bvec3 b) {
	return bvec3(a.x&&b.x, a.y&&b.y, a.z&&b.z);
}
inline bvec3 operator||(bvec3 a, bvec3 b) {
	return bvec3(a.x||b.x, a.y||b.y, a.z||b.z);
}
inline bvec3 operator!(bvec3 v) {
	return bvec3(!v.x, !v.y, !v.z);
}
inline bool all(bvec3 v) {
	return v.x&&v.y&&v.z;
}
inline bool any(bvec3 v) {
	return v.x||v.y||v.z;
}
inline bvec4 operator==(bvec4 a, bvec4 b) {
	return bvec4(a.x==b.x, a.y==b.y, a.z==b.z, a.w==b.w);
}
inline bvec4 operator!=(bvec4 a, bvec4 b) {
	return bvec4(a.x!=b.x, a.y!=b.y, a.z!=b.z, a.w!=b.w);
}
inline bvec4 operator&&(bvec4 a, bvec4 b) {
	return bvec4(a.x&&b.x, a.y&&b.y, a.z&&b.z, a.w&&b.w);
}
inline bvec4 operator||(bvec4 a, bvec4 b) {
	return bvec4(a.x||b.x, a.y||b.y, a.z||b.z, a.w||b.w);
}
inline bvec4 operator!(bvec4 v) {
	return bvec4(!v.x, !v.y, !v.z, !v.w);
}
inline bool all(bvec4 v) {
	return v.x&&v.y&&v.z&&v.w;
}
inline bool any(bvec4 v) {
	return v.x||v.y||v.z||v.w;
}
#pragma warning( default : 4146)
#include "maths_custom.h"
#include "maths_vector.h"