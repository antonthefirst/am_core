 #pragma once
#include <algorithm>
#include <cmath>
struct float2 {
float x; float y; 
};struct float3 {
float x; float y; float z; 
};struct float4 {
float x; float y; float z; float w; 
};struct double2 {
double x; double y; 
};struct double3 {
double x; double y; double z; 
};struct double4 {
double x; double y; double z; double w; 
};struct int2 {
int x; int y; 
};struct int3 {
int x; int y; int z; 
};struct int4 {
int x; int y; int z; int w; 
};struct uint2 {
uint x; uint y; 
};struct uint3 {
uint x; uint y; uint z; 
};struct uint4 {
uint x; uint y; uint z; uint w; 
};struct bool2 {
bool x; bool y; 
};struct bool3 {
bool x; bool y; bool z; 
};struct bool4 {
bool x; bool y; bool z; bool w; 
};inline float2 make_float2(float s) {
  return { s, s };
}inline float3 make_float3(float s) {
  return { s, s, s };
}inline float4 make_float4(float s) {
  return { s, s, s, s };
}inline double2 make_double2(double s) {
  return { s, s };
}inline double3 make_double3(double s) {
  return { s, s, s };
}inline double4 make_double4(double s) {
  return { s, s, s, s };
}inline int2 make_int2(int s) {
  return { s, s };
}inline int3 make_int3(int s) {
  return { s, s, s };
}inline int4 make_int4(int s) {
  return { s, s, s, s };
}inline uint2 make_uint2(uint s) {
  return { s, s };
}inline uint3 make_uint3(uint s) {
  return { s, s, s };
}inline uint4 make_uint4(uint s) {
  return { s, s, s, s };
}inline bool2 make_bool2(bool s) {
  return { s, s };
}inline bool3 make_bool3(bool s) {
  return { s, s, s };
}inline bool4 make_bool4(bool s) {
  return { s, s, s, s };
}inline float2 make_float2(double2 v) {
  return { static_cast<float>(v.x), static_cast<float>(v.y) };
}inline float2 make_float2(int2 v) {
  return { static_cast<float>(v.x), static_cast<float>(v.y) };
}inline float2 make_float2(uint2 v) {
  return { static_cast<float>(v.x), static_cast<float>(v.y) };
}inline float3 make_float3(double3 v) {
  return { static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z) };
}inline float3 make_float3(int3 v) {
  return { static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z) };
}inline float3 make_float3(uint3 v) {
  return { static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z) };
}inline float4 make_float4(double4 v) {
  return { static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z), static_cast<float>(v.w) };
}inline float4 make_float4(int4 v) {
  return { static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z), static_cast<float>(v.w) };
}inline float4 make_float4(uint4 v) {
  return { static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z), static_cast<float>(v.w) };
}inline double2 make_double2(float2 v) {
  return { static_cast<double>(v.x), static_cast<double>(v.y) };
}inline double2 make_double2(int2 v) {
  return { static_cast<double>(v.x), static_cast<double>(v.y) };
}inline double2 make_double2(uint2 v) {
  return { static_cast<double>(v.x), static_cast<double>(v.y) };
}inline double3 make_double3(float3 v) {
  return { static_cast<double>(v.x), static_cast<double>(v.y), static_cast<double>(v.z) };
}inline double3 make_double3(int3 v) {
  return { static_cast<double>(v.x), static_cast<double>(v.y), static_cast<double>(v.z) };
}inline double3 make_double3(uint3 v) {
  return { static_cast<double>(v.x), static_cast<double>(v.y), static_cast<double>(v.z) };
}inline double4 make_double4(float4 v) {
  return { static_cast<double>(v.x), static_cast<double>(v.y), static_cast<double>(v.z), static_cast<double>(v.w) };
}inline double4 make_double4(int4 v) {
  return { static_cast<double>(v.x), static_cast<double>(v.y), static_cast<double>(v.z), static_cast<double>(v.w) };
}inline double4 make_double4(uint4 v) {
  return { static_cast<double>(v.x), static_cast<double>(v.y), static_cast<double>(v.z), static_cast<double>(v.w) };
}inline int2 make_int2(float2 v) {
  return { static_cast<int>(v.x), static_cast<int>(v.y) };
}inline int2 make_int2(double2 v) {
  return { static_cast<int>(v.x), static_cast<int>(v.y) };
}inline int2 make_int2(uint2 v) {
  return { static_cast<int>(v.x), static_cast<int>(v.y) };
}inline int3 make_int3(float3 v) {
  return { static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z) };
}inline int3 make_int3(double3 v) {
  return { static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z) };
}inline int3 make_int3(uint3 v) {
  return { static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z) };
}inline int4 make_int4(float4 v) {
  return { static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z), static_cast<int>(v.w) };
}inline int4 make_int4(double4 v) {
  return { static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z), static_cast<int>(v.w) };
}inline int4 make_int4(uint4 v) {
  return { static_cast<int>(v.x), static_cast<int>(v.y), static_cast<int>(v.z), static_cast<int>(v.w) };
}inline uint2 make_uint2(float2 v) {
  return { static_cast<uint>(v.x), static_cast<uint>(v.y) };
}inline uint2 make_uint2(double2 v) {
  return { static_cast<uint>(v.x), static_cast<uint>(v.y) };
}inline uint2 make_uint2(int2 v) {
  return { static_cast<uint>(v.x), static_cast<uint>(v.y) };
}inline uint3 make_uint3(float3 v) {
  return { static_cast<uint>(v.x), static_cast<uint>(v.y), static_cast<uint>(v.z) };
}inline uint3 make_uint3(double3 v) {
  return { static_cast<uint>(v.x), static_cast<uint>(v.y), static_cast<uint>(v.z) };
}inline uint3 make_uint3(int3 v) {
  return { static_cast<uint>(v.x), static_cast<uint>(v.y), static_cast<uint>(v.z) };
}inline uint4 make_uint4(float4 v) {
  return { static_cast<uint>(v.x), static_cast<uint>(v.y), static_cast<uint>(v.z), static_cast<uint>(v.w) };
}inline uint4 make_uint4(double4 v) {
  return { static_cast<uint>(v.x), static_cast<uint>(v.y), static_cast<uint>(v.z), static_cast<uint>(v.w) };
}inline uint4 make_uint4(int4 v) {
  return { static_cast<uint>(v.x), static_cast<uint>(v.y), static_cast<uint>(v.z), static_cast<uint>(v.w) };
}inline float sq(float a) {
  return a * a;
}
inline float clamp(float a, float b, float c) {
  return std::min(std::max(a, b), c);
}inline float saturate(float a) {
  return clamp(a, 0.0f, 1.0f);
}
inline float sign(float a) {
  return (a > 0.0f) - (a < 0.0f);
}
inline float fract(float a) {
  return a - std::floor(a);
}
inline float mod(float a, float b) {
  return a - b * std::floor(a / b);
}
inline float step(float a) {
  return a < 0.0f ? 0.0f : 1.0f;
}
inline float step(float a, float b) {
  return a < b ? 0.0f : 1.0f;
}
inline float ramp(float a, float b, float c) {
  return saturate((c - a) / (b - a));
}
inline float smoothstep(float a) {
  return a * a * (3.0f - 2.0f * a);
}
inline float smoothstep(float a, float b, float c) {
  const float t = ramp(a, b, c);
  return t * t * (3.0f - 2.0f * t);
}
inline float smootherstep(float a) {
  return a * a * a * (a * (a * 6.0f - 15.0f) + 10.0f);
}
inline float smootherstep(float a, float b, float c) {
  const float t = ramp(a, b, c);
  return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}
inline float lerp(float a, float b, float c) {
  return a + (b - a) * c;
}inline float sum(float2 a) {
  return a.x + a.y;
}inline float product(float2 a) {
  return a.x * a.y;
}inline float min(float2 a) {
  return std::min(a.x, a.y);
}inline float max(float2 a) {
  return std::max(a.x, a.y);
}inline float2 operator-(float2 a) {
  return {-a.x, -a.y};
}inline float2 sq(float2 a) {
  return {sq(a.x), sq(a.y)};
}inline float2 operator+(float2 a, float2 b) {
  return {a.x + b.x, a.y + b.y};
}inline float2 operator-(float2 a, float2 b) {
  return {a.x - b.x, a.y - b.y};
}inline float2 operator*(float2 a, float2 b) {
  return {a.x * b.x, a.y * b.y};
}inline float2 operator/(float2 a, float2 b) {
  return {a.x / b.x, a.y / b.y};
}inline float2 min(float2 a, float2 b) {
  return {std::min(a.x, b.x), std::min(a.y, b.y)};
}inline float2 max(float2 a, float2 b) {
  return {std::max(a.x, b.x), std::max(a.y, b.y)};
}inline float2 clamp(float2 a, float2 b, float2 c) {
  return {clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y)};
}inline void operator+=(float2& a, float2 b) {
  a.x += b.x; a.y += b.y; 
}inline void operator-=(float2& a, float2 b) {
  a.x -= b.x; a.y -= b.y; 
}inline void operator*=(float2& a, float2 b) {
  a.x *= b.x; a.y *= b.y; 
}inline void operator/=(float2& a, float2 b) {
  a.x /= b.x; a.y /= b.y; 
}inline bool2 operator==(float2 a, float2 b) {
  return {a.x == b.x, a.y == b.y};
}inline bool2 operator!=(float2 a, float2 b) {
  return {a.x != b.x, a.y != b.y};
}inline bool2 operator<(float2 a, float2 b) {
  return {a.x < b.x, a.y < b.y};
}inline bool2 operator>(float2 a, float2 b) {
  return {a.x > b.x, a.y > b.y};
}inline bool2 operator<=(float2 a, float2 b) {
  return {a.x <= b.x, a.y <= b.y};
}inline bool2 operator>=(float2 a, float2 b) {
  return {a.x >= b.x, a.y >= b.y};
}inline float sum(float3 a) {
  return a.x + a.y + a.z;
}inline float product(float3 a) {
  return a.x * a.y * a.z;
}inline float min(float3 a) {
  return std::min(a.x, std::min(a.y, a.z));
}inline float max(float3 a) {
  return std::max(a.x, std::max(a.y, a.z));
}inline float3 operator-(float3 a) {
  return {-a.x, -a.y, -a.z};
}inline float3 sq(float3 a) {
  return {sq(a.x), sq(a.y), sq(a.z)};
}inline float3 operator+(float3 a, float3 b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z};
}inline float3 operator-(float3 a, float3 b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z};
}inline float3 operator*(float3 a, float3 b) {
  return {a.x * b.x, a.y * b.y, a.z * b.z};
}inline float3 operator/(float3 a, float3 b) {
  return {a.x / b.x, a.y / b.y, a.z / b.z};
}inline float3 min(float3 a, float3 b) {
  return {std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)};
}inline float3 max(float3 a, float3 b) {
  return {std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)};
}inline float3 clamp(float3 a, float3 b, float3 c) {
  return {clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y), clamp(a.z, b.z, c.z)};
}inline void operator+=(float3& a, float3 b) {
  a.x += b.x; a.y += b.y; a.z += b.z; 
}inline void operator-=(float3& a, float3 b) {
  a.x -= b.x; a.y -= b.y; a.z -= b.z; 
}inline void operator*=(float3& a, float3 b) {
  a.x *= b.x; a.y *= b.y; a.z *= b.z; 
}inline void operator/=(float3& a, float3 b) {
  a.x /= b.x; a.y /= b.y; a.z /= b.z; 
}inline bool3 operator==(float3 a, float3 b) {
  return {a.x == b.x, a.y == b.y, a.z == b.z};
}inline bool3 operator!=(float3 a, float3 b) {
  return {a.x != b.x, a.y != b.y, a.z != b.z};
}inline bool3 operator<(float3 a, float3 b) {
  return {a.x < b.x, a.y < b.y, a.z < b.z};
}inline bool3 operator>(float3 a, float3 b) {
  return {a.x > b.x, a.y > b.y, a.z > b.z};
}inline bool3 operator<=(float3 a, float3 b) {
  return {a.x <= b.x, a.y <= b.y, a.z <= b.z};
}inline bool3 operator>=(float3 a, float3 b) {
  return {a.x >= b.x, a.y >= b.y, a.z >= b.z};
}inline float sum(float4 a) {
  return a.x + a.y + a.z + a.w;
}inline float product(float4 a) {
  return a.x * a.y * a.z * a.w;
}inline float min(float4 a) {
  return std::min(a.x, std::min(a.y, std::min(a.z, a.w)));
}inline float max(float4 a) {
  return std::max(a.x, std::max(a.y, std::max(a.z, a.w)));
}inline float4 operator-(float4 a) {
  return {-a.x, -a.y, -a.z, -a.w};
}inline float4 sq(float4 a) {
  return {sq(a.x), sq(a.y), sq(a.z), sq(a.w)};
}inline float4 operator+(float4 a, float4 b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}inline float4 operator-(float4 a, float4 b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}inline float4 operator*(float4 a, float4 b) {
  return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}inline float4 operator/(float4 a, float4 b) {
  return {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}inline float4 min(float4 a, float4 b) {
  return {std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w)};
}inline float4 max(float4 a, float4 b) {
  return {std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w)};
}inline float4 clamp(float4 a, float4 b, float4 c) {
  return {clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y), clamp(a.z, b.z, c.z), clamp(a.w, b.w, c.w)};
}inline void operator+=(float4& a, float4 b) {
  a.x += b.x; a.y += b.y; a.z += b.z; a.w += b.w; 
}inline void operator-=(float4& a, float4 b) {
  a.x -= b.x; a.y -= b.y; a.z -= b.z; a.w -= b.w; 
}inline void operator*=(float4& a, float4 b) {
  a.x *= b.x; a.y *= b.y; a.z *= b.z; a.w *= b.w; 
}inline void operator/=(float4& a, float4 b) {
  a.x /= b.x; a.y /= b.y; a.z /= b.z; a.w /= b.w; 
}inline bool4 operator==(float4 a, float4 b) {
  return {a.x == b.x, a.y == b.y, a.z == b.z, a.w == b.w};
}inline bool4 operator!=(float4 a, float4 b) {
  return {a.x != b.x, a.y != b.y, a.z != b.z, a.w != b.w};
}inline bool4 operator<(float4 a, float4 b) {
  return {a.x < b.x, a.y < b.y, a.z < b.z, a.w < b.w};
}inline bool4 operator>(float4 a, float4 b) {
  return {a.x > b.x, a.y > b.y, a.z > b.z, a.w > b.w};
}inline bool4 operator<=(float4 a, float4 b) {
  return {a.x <= b.x, a.y <= b.y, a.z <= b.z, a.w <= b.w};
}inline bool4 operator>=(float4 a, float4 b) {
  return {a.x >= b.x, a.y >= b.y, a.z >= b.z, a.w >= b.w};
}inline float2 abs(float2 a) {
  return {std::abs(a.x), std::abs(a.y)};
}inline float2 sign(float2 a) {
  return {sign(a.x), sign(a.y)};
}inline float2 saturate(float2 a) {
  return {saturate(a.x), saturate(a.y)};
}inline float2 floor(float2 a) {
  return {std::floor(a.x), std::floor(a.y)};
}inline float2 ceil(float2 a) {
  return {std::ceil(a.x), std::ceil(a.y)};
}inline float2 fract(float2 a) {
  return {fract(a.x), fract(a.y)};
}inline float2 step(float2 a) {
  return {step(a.x), step(a.y)};
}inline float2 smoothstep(float2 a) {
  return {smoothstep(a.x), smoothstep(a.y)};
}inline float2 smootherstep(float2 a) {
  return {smootherstep(a.x), smootherstep(a.y)};
}inline float2 exp(float2 a) {
  return {std::exp(a.x), std::exp(a.y)};
}inline float2 log(float2 a) {
  return {std::log(a.x), std::log(a.y)};
}inline float2 sqrt(float2 a) {
  return {std::sqrt(a.x), std::sqrt(a.y)};
}inline float2 cos(float2 a) {
  return {std::cos(a.x), std::cos(a.y)};
}inline float2 sin(float2 a) {
  return {std::sin(a.x), std::sin(a.y)};
}inline float2 tan(float2 a) {
  return {std::tan(a.x), std::tan(a.y)};
}inline float2 acos(float2 a) {
  return {std::acos(a.x), std::acos(a.y)};
}inline float2 asin(float2 a) {
  return {std::asin(a.x), std::asin(a.y)};
}inline float2 atan(float2 a) {
  return {std::atan(a.x), std::atan(a.y)};
}inline float2 mod(float2 a, float2 b) {
  return {mod(a.x, b.x), mod(a.y, b.y)};
}inline float2 pow(float2 a, float2 b) {
  return {std::pow(a.x, b.x), std::pow(a.y, b.y)};
}inline float2 step(float2 a, float2 b) {
  return {step(a.x, b.x), step(a.y, b.y)};
}inline float2 ramp(float2 a, float2 b, float2 c) {
  return {ramp(a.x, b.x, c.x), ramp(a.y, b.y, c.y)};
}inline float2 smoothstep(float2 a, float2 b, float2 c) {
  return {smoothstep(a.x, b.x, c.x), smoothstep(a.y, b.y, c.y)};
}inline float2 smootherstep(float2 a, float2 b, float2 c) {
  return {smootherstep(a.x, b.x, c.x), smootherstep(a.y, b.y, c.y)};
}inline float2 lerp(float2 a, float2 b, float2 c) {
  return {lerp(a.x, b.x, c.x), lerp(a.y, b.y, c.y)};
}inline bool2 isnan(float2 a) {
  return {std::isnan(a.x), std::isnan(a.y)};
}inline float3 abs(float3 a) {
  return {std::abs(a.x), std::abs(a.y), std::abs(a.z)};
}inline float3 sign(float3 a) {
  return {sign(a.x), sign(a.y), sign(a.z)};
}inline float3 saturate(float3 a) {
  return {saturate(a.x), saturate(a.y), saturate(a.z)};
}inline float3 floor(float3 a) {
  return {std::floor(a.x), std::floor(a.y), std::floor(a.z)};
}inline float3 ceil(float3 a) {
  return {std::ceil(a.x), std::ceil(a.y), std::ceil(a.z)};
}inline float3 fract(float3 a) {
  return {fract(a.x), fract(a.y), fract(a.z)};
}inline float3 step(float3 a) {
  return {step(a.x), step(a.y), step(a.z)};
}inline float3 smoothstep(float3 a) {
  return {smoothstep(a.x), smoothstep(a.y), smoothstep(a.z)};
}inline float3 smootherstep(float3 a) {
  return {smootherstep(a.x), smootherstep(a.y), smootherstep(a.z)};
}inline float3 exp(float3 a) {
  return {std::exp(a.x), std::exp(a.y), std::exp(a.z)};
}inline float3 log(float3 a) {
  return {std::log(a.x), std::log(a.y), std::log(a.z)};
}inline float3 sqrt(float3 a) {
  return {std::sqrt(a.x), std::sqrt(a.y), std::sqrt(a.z)};
}inline float3 cos(float3 a) {
  return {std::cos(a.x), std::cos(a.y), std::cos(a.z)};
}inline float3 sin(float3 a) {
  return {std::sin(a.x), std::sin(a.y), std::sin(a.z)};
}inline float3 tan(float3 a) {
  return {std::tan(a.x), std::tan(a.y), std::tan(a.z)};
}inline float3 acos(float3 a) {
  return {std::acos(a.x), std::acos(a.y), std::acos(a.z)};
}inline float3 asin(float3 a) {
  return {std::asin(a.x), std::asin(a.y), std::asin(a.z)};
}inline float3 atan(float3 a) {
  return {std::atan(a.x), std::atan(a.y), std::atan(a.z)};
}inline float3 mod(float3 a, float3 b) {
  return {mod(a.x, b.x), mod(a.y, b.y), mod(a.z, b.z)};
}inline float3 pow(float3 a, float3 b) {
  return {std::pow(a.x, b.x), std::pow(a.y, b.y), std::pow(a.z, b.z)};
}inline float3 step(float3 a, float3 b) {
  return {step(a.x, b.x), step(a.y, b.y), step(a.z, b.z)};
}inline float3 ramp(float3 a, float3 b, float3 c) {
  return {ramp(a.x, b.x, c.x), ramp(a.y, b.y, c.y), ramp(a.z, b.z, c.z)};
}inline float3 smoothstep(float3 a, float3 b, float3 c) {
  return {smoothstep(a.x, b.x, c.x), smoothstep(a.y, b.y, c.y), smoothstep(a.z, b.z, c.z)};
}inline float3 smootherstep(float3 a, float3 b, float3 c) {
  return {smootherstep(a.x, b.x, c.x), smootherstep(a.y, b.y, c.y), smootherstep(a.z, b.z, c.z)};
}inline float3 lerp(float3 a, float3 b, float3 c) {
  return {lerp(a.x, b.x, c.x), lerp(a.y, b.y, c.y), lerp(a.z, b.z, c.z)};
}inline bool3 isnan(float3 a) {
  return {std::isnan(a.x), std::isnan(a.y), std::isnan(a.z)};
}inline float4 abs(float4 a) {
  return {std::abs(a.x), std::abs(a.y), std::abs(a.z), std::abs(a.w)};
}inline float4 sign(float4 a) {
  return {sign(a.x), sign(a.y), sign(a.z), sign(a.w)};
}inline float4 saturate(float4 a) {
  return {saturate(a.x), saturate(a.y), saturate(a.z), saturate(a.w)};
}inline float4 floor(float4 a) {
  return {std::floor(a.x), std::floor(a.y), std::floor(a.z), std::floor(a.w)};
}inline float4 ceil(float4 a) {
  return {std::ceil(a.x), std::ceil(a.y), std::ceil(a.z), std::ceil(a.w)};
}inline float4 fract(float4 a) {
  return {fract(a.x), fract(a.y), fract(a.z), fract(a.w)};
}inline float4 step(float4 a) {
  return {step(a.x), step(a.y), step(a.z), step(a.w)};
}inline float4 smoothstep(float4 a) {
  return {smoothstep(a.x), smoothstep(a.y), smoothstep(a.z), smoothstep(a.w)};
}inline float4 smootherstep(float4 a) {
  return {smootherstep(a.x), smootherstep(a.y), smootherstep(a.z), smootherstep(a.w)};
}inline float4 exp(float4 a) {
  return {std::exp(a.x), std::exp(a.y), std::exp(a.z), std::exp(a.w)};
}inline float4 log(float4 a) {
  return {std::log(a.x), std::log(a.y), std::log(a.z), std::log(a.w)};
}inline float4 sqrt(float4 a) {
  return {std::sqrt(a.x), std::sqrt(a.y), std::sqrt(a.z), std::sqrt(a.w)};
}inline float4 cos(float4 a) {
  return {std::cos(a.x), std::cos(a.y), std::cos(a.z), std::cos(a.w)};
}inline float4 sin(float4 a) {
  return {std::sin(a.x), std::sin(a.y), std::sin(a.z), std::sin(a.w)};
}inline float4 tan(float4 a) {
  return {std::tan(a.x), std::tan(a.y), std::tan(a.z), std::tan(a.w)};
}inline float4 acos(float4 a) {
  return {std::acos(a.x), std::acos(a.y), std::acos(a.z), std::acos(a.w)};
}inline float4 asin(float4 a) {
  return {std::asin(a.x), std::asin(a.y), std::asin(a.z), std::asin(a.w)};
}inline float4 atan(float4 a) {
  return {std::atan(a.x), std::atan(a.y), std::atan(a.z), std::atan(a.w)};
}inline float4 mod(float4 a, float4 b) {
  return {mod(a.x, b.x), mod(a.y, b.y), mod(a.z, b.z), mod(a.w, b.w)};
}inline float4 pow(float4 a, float4 b) {
  return {std::pow(a.x, b.x), std::pow(a.y, b.y), std::pow(a.z, b.z), std::pow(a.w, b.w)};
}inline float4 step(float4 a, float4 b) {
  return {step(a.x, b.x), step(a.y, b.y), step(a.z, b.z), step(a.w, b.w)};
}inline float4 ramp(float4 a, float4 b, float4 c) {
  return {ramp(a.x, b.x, c.x), ramp(a.y, b.y, c.y), ramp(a.z, b.z, c.z), ramp(a.w, b.w, c.w)};
}inline float4 smoothstep(float4 a, float4 b, float4 c) {
  return {smoothstep(a.x, b.x, c.x), smoothstep(a.y, b.y, c.y), smoothstep(a.z, b.z, c.z), smoothstep(a.w, b.w, c.w)};
}inline float4 smootherstep(float4 a, float4 b, float4 c) {
  return {smootherstep(a.x, b.x, c.x), smootherstep(a.y, b.y, c.y), smootherstep(a.z, b.z, c.z), smootherstep(a.w, b.w, c.w)};
}inline float4 lerp(float4 a, float4 b, float4 c) {
  return {lerp(a.x, b.x, c.x), lerp(a.y, b.y, c.y), lerp(a.z, b.z, c.z), lerp(a.w, b.w, c.w)};
}inline bool4 isnan(float4 a) {
  return {std::isnan(a.x), std::isnan(a.y), std::isnan(a.z), std::isnan(a.w)};
}inline float dot(float2 a, float2 b) {
  return sum(a * b);
}
inline float lengthsq(float2 a) {
  return dot(a, a);
}
inline float length(float2 a) {
  return sqrtf(lengthsq(a));
}
inline float2 normalize(float2 a) {
  return a / make_float2(length(a));
}
inline float2 normalize_or_0(float2 a) {
  float len = length(a);
  return len > 0.0f ? a / make_float2(len): make_float2(0.0f);
}inline float dot(float3 a, float3 b) {
  return sum(a * b);
}
inline float lengthsq(float3 a) {
  return dot(a, a);
}
inline float length(float3 a) {
  return sqrtf(lengthsq(a));
}
inline float3 normalize(float3 a) {
  return a / make_float3(length(a));
}
inline float3 normalize_or_0(float3 a) {
  float len = length(a);
  return len > 0.0f ? a / make_float3(len): make_float3(0.0f);
}inline float dot(float4 a, float4 b) {
  return sum(a * b);
}
inline float lengthsq(float4 a) {
  return dot(a, a);
}
inline float length(float4 a) {
  return sqrtf(lengthsq(a));
}
inline float4 normalize(float4 a) {
  return a / make_float4(length(a));
}
inline float4 normalize_or_0(float4 a) {
  float len = length(a);
  return len > 0.0f ? a / make_float4(len): make_float4(0.0f);
}inline float det(float2 a, float2 b) {
  return a.x * b.y - a.y * b.x;
}inline float3 cross(float3 a, float3 b) {
  return {a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y};
}
inline float det(float3 a, float3 b) {
  return length(cross(a, b));
}inline int sq(int a) {
  return a * a;
}
inline int clamp(int a, int b, int c) {
  return std::min(std::max(a, b), c);
}inline int sign(int a) {
  return (a > 0.0) - (a < 0.0);
}
inline int abs(int a) {
  return std::abs(a);
}inline int sum(int2 a) {
  return a.x + a.y;
}inline int product(int2 a) {
  return a.x * a.y;
}inline int min(int2 a) {
  return std::min(a.x, a.y);
}inline int max(int2 a) {
  return std::max(a.x, a.y);
}inline int2 operator-(int2 a) {
  return {-a.x, -a.y};
}inline int2 sq(int2 a) {
  return {sq(a.x), sq(a.y)};
}inline int2 operator+(int2 a, int2 b) {
  return {a.x + b.x, a.y + b.y};
}inline int2 operator-(int2 a, int2 b) {
  return {a.x - b.x, a.y - b.y};
}inline int2 operator*(int2 a, int2 b) {
  return {a.x * b.x, a.y * b.y};
}inline int2 operator/(int2 a, int2 b) {
  return {a.x / b.x, a.y / b.y};
}inline int2 min(int2 a, int2 b) {
  return {std::min(a.x, b.x), std::min(a.y, b.y)};
}inline int2 max(int2 a, int2 b) {
  return {std::max(a.x, b.x), std::max(a.y, b.y)};
}inline int2 clamp(int2 a, int2 b, int2 c) {
  return {clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y)};
}inline void operator+=(int2& a, int2 b) {
  a.x += b.x; a.y += b.y; 
}inline void operator-=(int2& a, int2 b) {
  a.x -= b.x; a.y -= b.y; 
}inline void operator*=(int2& a, int2 b) {
  a.x *= b.x; a.y *= b.y; 
}inline void operator/=(int2& a, int2 b) {
  a.x /= b.x; a.y /= b.y; 
}inline bool2 operator==(int2 a, int2 b) {
  return {a.x == b.x, a.y == b.y};
}inline bool2 operator!=(int2 a, int2 b) {
  return {a.x != b.x, a.y != b.y};
}inline bool2 operator<(int2 a, int2 b) {
  return {a.x < b.x, a.y < b.y};
}inline bool2 operator>(int2 a, int2 b) {
  return {a.x > b.x, a.y > b.y};
}inline bool2 operator<=(int2 a, int2 b) {
  return {a.x <= b.x, a.y <= b.y};
}inline bool2 operator>=(int2 a, int2 b) {
  return {a.x >= b.x, a.y >= b.y};
}inline int sum(int3 a) {
  return a.x + a.y + a.z;
}inline int product(int3 a) {
  return a.x * a.y * a.z;
}inline int min(int3 a) {
  return std::min(a.x, std::min(a.y, a.z));
}inline int max(int3 a) {
  return std::max(a.x, std::max(a.y, a.z));
}inline int3 operator-(int3 a) {
  return {-a.x, -a.y, -a.z};
}inline int3 sq(int3 a) {
  return {sq(a.x), sq(a.y), sq(a.z)};
}inline int3 operator+(int3 a, int3 b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z};
}inline int3 operator-(int3 a, int3 b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z};
}inline int3 operator*(int3 a, int3 b) {
  return {a.x * b.x, a.y * b.y, a.z * b.z};
}inline int3 operator/(int3 a, int3 b) {
  return {a.x / b.x, a.y / b.y, a.z / b.z};
}inline int3 min(int3 a, int3 b) {
  return {std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)};
}inline int3 max(int3 a, int3 b) {
  return {std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)};
}inline int3 clamp(int3 a, int3 b, int3 c) {
  return {clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y), clamp(a.z, b.z, c.z)};
}inline void operator+=(int3& a, int3 b) {
  a.x += b.x; a.y += b.y; a.z += b.z; 
}inline void operator-=(int3& a, int3 b) {
  a.x -= b.x; a.y -= b.y; a.z -= b.z; 
}inline void operator*=(int3& a, int3 b) {
  a.x *= b.x; a.y *= b.y; a.z *= b.z; 
}inline void operator/=(int3& a, int3 b) {
  a.x /= b.x; a.y /= b.y; a.z /= b.z; 
}inline bool3 operator==(int3 a, int3 b) {
  return {a.x == b.x, a.y == b.y, a.z == b.z};
}inline bool3 operator!=(int3 a, int3 b) {
  return {a.x != b.x, a.y != b.y, a.z != b.z};
}inline bool3 operator<(int3 a, int3 b) {
  return {a.x < b.x, a.y < b.y, a.z < b.z};
}inline bool3 operator>(int3 a, int3 b) {
  return {a.x > b.x, a.y > b.y, a.z > b.z};
}inline bool3 operator<=(int3 a, int3 b) {
  return {a.x <= b.x, a.y <= b.y, a.z <= b.z};
}inline bool3 operator>=(int3 a, int3 b) {
  return {a.x >= b.x, a.y >= b.y, a.z >= b.z};
}inline int sum(int4 a) {
  return a.x + a.y + a.z + a.w;
}inline int product(int4 a) {
  return a.x * a.y * a.z * a.w;
}inline int min(int4 a) {
  return std::min(a.x, std::min(a.y, std::min(a.z, a.w)));
}inline int max(int4 a) {
  return std::max(a.x, std::max(a.y, std::max(a.z, a.w)));
}inline int4 operator-(int4 a) {
  return {-a.x, -a.y, -a.z, -a.w};
}inline int4 sq(int4 a) {
  return {sq(a.x), sq(a.y), sq(a.z), sq(a.w)};
}inline int4 operator+(int4 a, int4 b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}inline int4 operator-(int4 a, int4 b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}inline int4 operator*(int4 a, int4 b) {
  return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}inline int4 operator/(int4 a, int4 b) {
  return {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}inline int4 min(int4 a, int4 b) {
  return {std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w)};
}inline int4 max(int4 a, int4 b) {
  return {std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w)};
}inline int4 clamp(int4 a, int4 b, int4 c) {
  return {clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y), clamp(a.z, b.z, c.z), clamp(a.w, b.w, c.w)};
}inline void operator+=(int4& a, int4 b) {
  a.x += b.x; a.y += b.y; a.z += b.z; a.w += b.w; 
}inline void operator-=(int4& a, int4 b) {
  a.x -= b.x; a.y -= b.y; a.z -= b.z; a.w -= b.w; 
}inline void operator*=(int4& a, int4 b) {
  a.x *= b.x; a.y *= b.y; a.z *= b.z; a.w *= b.w; 
}inline void operator/=(int4& a, int4 b) {
  a.x /= b.x; a.y /= b.y; a.z /= b.z; a.w /= b.w; 
}inline bool4 operator==(int4 a, int4 b) {
  return {a.x == b.x, a.y == b.y, a.z == b.z, a.w == b.w};
}inline bool4 operator!=(int4 a, int4 b) {
  return {a.x != b.x, a.y != b.y, a.z != b.z, a.w != b.w};
}inline bool4 operator<(int4 a, int4 b) {
  return {a.x < b.x, a.y < b.y, a.z < b.z, a.w < b.w};
}inline bool4 operator>(int4 a, int4 b) {
  return {a.x > b.x, a.y > b.y, a.z > b.z, a.w > b.w};
}inline bool4 operator<=(int4 a, int4 b) {
  return {a.x <= b.x, a.y <= b.y, a.z <= b.z, a.w <= b.w};
}inline bool4 operator>=(int4 a, int4 b) {
  return {a.x >= b.x, a.y >= b.y, a.z >= b.z, a.w >= b.w};
}inline int2 operator~(int2 a) {
  return {~a.x, ~a.y};
}inline int2 operator|(int2 a, int2 b) {
  return {a.x | b.x, a.y | b.y};
}inline int2 operator&(int2 a, int2 b) {
  return {a.x & b.x, a.y & b.y};
}inline int2 operator^(int2 a, int2 b) {
  return {a.x ^ b.x, a.y ^ b.y};
}inline void operator|=(int2& a, int2 b) {
  a.x |= b.x; a.y |= b.y; 
}inline void operator&=(int2& a, int2 b) {
  a.x &= b.x; a.y &= b.y; 
}inline void operator^=(int2& a, int2 b) {
  a.x ^= b.x; a.y ^= b.y; 
}inline int3 operator~(int3 a) {
  return {~a.x, ~a.y, ~a.z};
}inline int3 operator|(int3 a, int3 b) {
  return {a.x | b.x, a.y | b.y, a.z | b.z};
}inline int3 operator&(int3 a, int3 b) {
  return {a.x & b.x, a.y & b.y, a.z & b.z};
}inline int3 operator^(int3 a, int3 b) {
  return {a.x ^ b.x, a.y ^ b.y, a.z ^ b.z};
}inline void operator|=(int3& a, int3 b) {
  a.x |= b.x; a.y |= b.y; a.z |= b.z; 
}inline void operator&=(int3& a, int3 b) {
  a.x &= b.x; a.y &= b.y; a.z &= b.z; 
}inline void operator^=(int3& a, int3 b) {
  a.x ^= b.x; a.y ^= b.y; a.z ^= b.z; 
}inline int4 operator~(int4 a) {
  return {~a.x, ~a.y, ~a.z, ~a.w};
}inline int4 operator|(int4 a, int4 b) {
  return {a.x | b.x, a.y | b.y, a.z | b.z, a.w | b.w};
}inline int4 operator&(int4 a, int4 b) {
  return {a.x & b.x, a.y & b.y, a.z & b.z, a.w & b.w};
}inline int4 operator^(int4 a, int4 b) {
  return {a.x ^ b.x, a.y ^ b.y, a.z ^ b.z, a.w ^ b.w};
}inline void operator|=(int4& a, int4 b) {
  a.x |= b.x; a.y |= b.y; a.z |= b.z; a.w |= b.w; 
}inline void operator&=(int4& a, int4 b) {
  a.x &= b.x; a.y &= b.y; a.z &= b.z; a.w &= b.w; 
}inline void operator^=(int4& a, int4 b) {
  a.x ^= b.x; a.y ^= b.y; a.z ^= b.z; a.w ^= b.w; 
}inline int2 abs(int2 a) {
  return {std::abs(a.x), std::abs(a.y)};
}inline int2 sign(int2 a) {
  return {sign(a.x), sign(a.y)};
}inline int3 abs(int3 a) {
  return {std::abs(a.x), std::abs(a.y), std::abs(a.z)};
}inline int3 sign(int3 a) {
  return {sign(a.x), sign(a.y), sign(a.z)};
}inline int4 abs(int4 a) {
  return {std::abs(a.x), std::abs(a.y), std::abs(a.z), std::abs(a.w)};
}inline int4 sign(int4 a) {
  return {sign(a.x), sign(a.y), sign(a.z), sign(a.w)};
}inline uint sq(uint a) {
  return a * a;
}
inline uint clamp(uint a, uint b, uint c) {
  return std::min(std::max(a, b), c);
}inline uint sum(uint2 a) {
  return a.x + a.y;
}inline uint product(uint2 a) {
  return a.x * a.y;
}inline uint min(uint2 a) {
  return std::min(a.x, a.y);
}inline uint max(uint2 a) {
  return std::max(a.x, a.y);
}inline uint2 operator-(uint2 a) {
  return {-a.x, -a.y};
}inline uint2 sq(uint2 a) {
  return {sq(a.x), sq(a.y)};
}inline uint2 operator+(uint2 a, uint2 b) {
  return {a.x + b.x, a.y + b.y};
}inline uint2 operator-(uint2 a, uint2 b) {
  return {a.x - b.x, a.y - b.y};
}inline uint2 operator*(uint2 a, uint2 b) {
  return {a.x * b.x, a.y * b.y};
}inline uint2 operator/(uint2 a, uint2 b) {
  return {a.x / b.x, a.y / b.y};
}inline uint2 min(uint2 a, uint2 b) {
  return {std::min(a.x, b.x), std::min(a.y, b.y)};
}inline uint2 max(uint2 a, uint2 b) {
  return {std::max(a.x, b.x), std::max(a.y, b.y)};
}inline uint2 clamp(uint2 a, uint2 b, uint2 c) {
  return {clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y)};
}inline void operator+=(uint2& a, uint2 b) {
  a.x += b.x; a.y += b.y; 
}inline void operator-=(uint2& a, uint2 b) {
  a.x -= b.x; a.y -= b.y; 
}inline void operator*=(uint2& a, uint2 b) {
  a.x *= b.x; a.y *= b.y; 
}inline void operator/=(uint2& a, uint2 b) {
  a.x /= b.x; a.y /= b.y; 
}inline bool2 operator==(uint2 a, uint2 b) {
  return {a.x == b.x, a.y == b.y};
}inline bool2 operator!=(uint2 a, uint2 b) {
  return {a.x != b.x, a.y != b.y};
}inline bool2 operator<(uint2 a, uint2 b) {
  return {a.x < b.x, a.y < b.y};
}inline bool2 operator>(uint2 a, uint2 b) {
  return {a.x > b.x, a.y > b.y};
}inline bool2 operator<=(uint2 a, uint2 b) {
  return {a.x <= b.x, a.y <= b.y};
}inline bool2 operator>=(uint2 a, uint2 b) {
  return {a.x >= b.x, a.y >= b.y};
}inline uint sum(uint3 a) {
  return a.x + a.y + a.z;
}inline uint product(uint3 a) {
  return a.x * a.y * a.z;
}inline uint min(uint3 a) {
  return std::min(a.x, std::min(a.y, a.z));
}inline uint max(uint3 a) {
  return std::max(a.x, std::max(a.y, a.z));
}inline uint3 operator-(uint3 a) {
  return {-a.x, -a.y, -a.z};
}inline uint3 sq(uint3 a) {
  return {sq(a.x), sq(a.y), sq(a.z)};
}inline uint3 operator+(uint3 a, uint3 b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z};
}inline uint3 operator-(uint3 a, uint3 b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z};
}inline uint3 operator*(uint3 a, uint3 b) {
  return {a.x * b.x, a.y * b.y, a.z * b.z};
}inline uint3 operator/(uint3 a, uint3 b) {
  return {a.x / b.x, a.y / b.y, a.z / b.z};
}inline uint3 min(uint3 a, uint3 b) {
  return {std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z)};
}inline uint3 max(uint3 a, uint3 b) {
  return {std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z)};
}inline uint3 clamp(uint3 a, uint3 b, uint3 c) {
  return {clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y), clamp(a.z, b.z, c.z)};
}inline void operator+=(uint3& a, uint3 b) {
  a.x += b.x; a.y += b.y; a.z += b.z; 
}inline void operator-=(uint3& a, uint3 b) {
  a.x -= b.x; a.y -= b.y; a.z -= b.z; 
}inline void operator*=(uint3& a, uint3 b) {
  a.x *= b.x; a.y *= b.y; a.z *= b.z; 
}inline void operator/=(uint3& a, uint3 b) {
  a.x /= b.x; a.y /= b.y; a.z /= b.z; 
}inline bool3 operator==(uint3 a, uint3 b) {
  return {a.x == b.x, a.y == b.y, a.z == b.z};
}inline bool3 operator!=(uint3 a, uint3 b) {
  return {a.x != b.x, a.y != b.y, a.z != b.z};
}inline bool3 operator<(uint3 a, uint3 b) {
  return {a.x < b.x, a.y < b.y, a.z < b.z};
}inline bool3 operator>(uint3 a, uint3 b) {
  return {a.x > b.x, a.y > b.y, a.z > b.z};
}inline bool3 operator<=(uint3 a, uint3 b) {
  return {a.x <= b.x, a.y <= b.y, a.z <= b.z};
}inline bool3 operator>=(uint3 a, uint3 b) {
  return {a.x >= b.x, a.y >= b.y, a.z >= b.z};
}inline uint sum(uint4 a) {
  return a.x + a.y + a.z + a.w;
}inline uint product(uint4 a) {
  return a.x * a.y * a.z * a.w;
}inline uint min(uint4 a) {
  return std::min(a.x, std::min(a.y, std::min(a.z, a.w)));
}inline uint max(uint4 a) {
  return std::max(a.x, std::max(a.y, std::max(a.z, a.w)));
}inline uint4 operator-(uint4 a) {
  return {-a.x, -a.y, -a.z, -a.w};
}inline uint4 sq(uint4 a) {
  return {sq(a.x), sq(a.y), sq(a.z), sq(a.w)};
}inline uint4 operator+(uint4 a, uint4 b) {
  return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}inline uint4 operator-(uint4 a, uint4 b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}inline uint4 operator*(uint4 a, uint4 b) {
  return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}inline uint4 operator/(uint4 a, uint4 b) {
  return {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}inline uint4 min(uint4 a, uint4 b) {
  return {std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w)};
}inline uint4 max(uint4 a, uint4 b) {
  return {std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w)};
}inline uint4 clamp(uint4 a, uint4 b, uint4 c) {
  return {clamp(a.x, b.x, c.x), clamp(a.y, b.y, c.y), clamp(a.z, b.z, c.z), clamp(a.w, b.w, c.w)};
}inline void operator+=(uint4& a, uint4 b) {
  a.x += b.x; a.y += b.y; a.z += b.z; a.w += b.w; 
}inline void operator-=(uint4& a, uint4 b) {
  a.x -= b.x; a.y -= b.y; a.z -= b.z; a.w -= b.w; 
}inline void operator*=(uint4& a, uint4 b) {
  a.x *= b.x; a.y *= b.y; a.z *= b.z; a.w *= b.w; 
}inline void operator/=(uint4& a, uint4 b) {
  a.x /= b.x; a.y /= b.y; a.z /= b.z; a.w /= b.w; 
}inline bool4 operator==(uint4 a, uint4 b) {
  return {a.x == b.x, a.y == b.y, a.z == b.z, a.w == b.w};
}inline bool4 operator!=(uint4 a, uint4 b) {
  return {a.x != b.x, a.y != b.y, a.z != b.z, a.w != b.w};
}inline bool4 operator<(uint4 a, uint4 b) {
  return {a.x < b.x, a.y < b.y, a.z < b.z, a.w < b.w};
}inline bool4 operator>(uint4 a, uint4 b) {
  return {a.x > b.x, a.y > b.y, a.z > b.z, a.w > b.w};
}inline bool4 operator<=(uint4 a, uint4 b) {
  return {a.x <= b.x, a.y <= b.y, a.z <= b.z, a.w <= b.w};
}inline bool4 operator>=(uint4 a, uint4 b) {
  return {a.x >= b.x, a.y >= b.y, a.z >= b.z, a.w >= b.w};
}inline uint2 operator~(uint2 a) {
  return {~a.x, ~a.y};
}inline uint2 operator|(uint2 a, uint2 b) {
  return {a.x | b.x, a.y | b.y};
}inline uint2 operator&(uint2 a, uint2 b) {
  return {a.x & b.x, a.y & b.y};
}inline uint2 operator^(uint2 a, uint2 b) {
  return {a.x ^ b.x, a.y ^ b.y};
}inline void operator|=(uint2& a, uint2 b) {
  a.x |= b.x; a.y |= b.y; 
}inline void operator&=(uint2& a, uint2 b) {
  a.x &= b.x; a.y &= b.y; 
}inline void operator^=(uint2& a, uint2 b) {
  a.x ^= b.x; a.y ^= b.y; 
}inline uint3 operator~(uint3 a) {
  return {~a.x, ~a.y, ~a.z};
}inline uint3 operator|(uint3 a, uint3 b) {
  return {a.x | b.x, a.y | b.y, a.z | b.z};
}inline uint3 operator&(uint3 a, uint3 b) {
  return {a.x & b.x, a.y & b.y, a.z & b.z};
}inline uint3 operator^(uint3 a, uint3 b) {
  return {a.x ^ b.x, a.y ^ b.y, a.z ^ b.z};
}inline void operator|=(uint3& a, uint3 b) {
  a.x |= b.x; a.y |= b.y; a.z |= b.z; 
}inline void operator&=(uint3& a, uint3 b) {
  a.x &= b.x; a.y &= b.y; a.z &= b.z; 
}inline void operator^=(uint3& a, uint3 b) {
  a.x ^= b.x; a.y ^= b.y; a.z ^= b.z; 
}inline uint4 operator~(uint4 a) {
  return {~a.x, ~a.y, ~a.z, ~a.w};
}inline uint4 operator|(uint4 a, uint4 b) {
  return {a.x | b.x, a.y | b.y, a.z | b.z, a.w | b.w};
}inline uint4 operator&(uint4 a, uint4 b) {
  return {a.x & b.x, a.y & b.y, a.z & b.z, a.w & b.w};
}inline uint4 operator^(uint4 a, uint4 b) {
  return {a.x ^ b.x, a.y ^ b.y, a.z ^ b.z, a.w ^ b.w};
}inline void operator|=(uint4& a, uint4 b) {
  a.x |= b.x; a.y |= b.y; a.z |= b.z; a.w |= b.w; 
}inline void operator&=(uint4& a, uint4 b) {
  a.x &= b.x; a.y &= b.y; a.z &= b.z; a.w &= b.w; 
}inline void operator^=(uint4& a, uint4 b) {
  a.x ^= b.x; a.y ^= b.y; a.z ^= b.z; a.w ^= b.w; 
}inline bool any(bool2 a) {
  return a.x || a.y;
}inline bool all(bool2 a) {
  return a.x && a.y;
}inline bool2 operator!(bool2 a) {
  return {!a.x, !a.y};
}inline bool2 operator==(bool2 a, bool2 b) {
  return {a.x == b.x, a.y == b.y};
}inline bool2 operator!=(bool2 a, bool2 b) {
  return {a.x != b.x, a.y != b.y};
}inline bool2 operator||(bool2 a, bool2 b) {
  return {a.x || b.x, a.y || b.y};
}inline bool2 operator&&(bool2 a, bool2 b) {
  return {a.x && b.x, a.y && b.y};
}inline void operator|=(bool2& a, bool2 b) {
  a.x |= b.x; a.y |= b.y; 
}inline void operator&=(bool2& a, bool2 b) {
  a.x &= b.x; a.y &= b.y; 
}inline bool any(bool3 a) {
  return a.x || a.y || a.z;
}inline bool all(bool3 a) {
  return a.x && a.y && a.z;
}inline bool3 operator!(bool3 a) {
  return {!a.x, !a.y, !a.z};
}inline bool3 operator==(bool3 a, bool3 b) {
  return {a.x == b.x, a.y == b.y, a.z == b.z};
}inline bool3 operator!=(bool3 a, bool3 b) {
  return {a.x != b.x, a.y != b.y, a.z != b.z};
}inline bool3 operator||(bool3 a, bool3 b) {
  return {a.x || b.x, a.y || b.y, a.z || b.z};
}inline bool3 operator&&(bool3 a, bool3 b) {
  return {a.x && b.x, a.y && b.y, a.z && b.z};
}inline void operator|=(bool3& a, bool3 b) {
  a.x |= b.x; a.y |= b.y; a.z |= b.z; 
}inline void operator&=(bool3& a, bool3 b) {
  a.x &= b.x; a.y &= b.y; a.z &= b.z; 
}inline bool any(bool4 a) {
  return a.x || a.y || a.z || a.w;
}inline bool all(bool4 a) {
  return a.x && a.y && a.z && a.w;
}inline bool4 operator!(bool4 a) {
  return {!a.x, !a.y, !a.z, !a.w};
}inline bool4 operator==(bool4 a, bool4 b) {
  return {a.x == b.x, a.y == b.y, a.z == b.z, a.w == b.w};
}inline bool4 operator!=(bool4 a, bool4 b) {
  return {a.x != b.x, a.y != b.y, a.z != b.z, a.w != b.w};
}inline bool4 operator||(bool4 a, bool4 b) {
  return {a.x || b.x, a.y || b.y, a.z || b.z, a.w || b.w};
}inline bool4 operator&&(bool4 a, bool4 b) {
  return {a.x && b.x, a.y && b.y, a.z && b.z, a.w && b.w};
}inline void operator|=(bool4& a, bool4 b) {
  a.x |= b.x; a.y |= b.y; a.z |= b.z; a.w |= b.w; 
}inline void operator&=(bool4& a, bool4 b) {
  a.x &= b.x; a.y &= b.y; a.z &= b.z; a.w &= b.w; 
}