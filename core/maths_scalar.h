#pragma once
#include <cmath>
#include <algorithm>

/* Constants */

#define EXP 2.71828182845904523536f
#define LOG2E 1.44269504088896340736f
#define LOG10E 0.434294481903251827651f
#define LN2 0.693147180559945309417f
#define LN10 2.30258509299404568402f
#define PI 3.14159265358979323846f
#define PI_2 1.57079632679489661923f
#define PI_4 0.785398163397448309616f
#define SQRT2 1.41421356237309504880f
#define SQRT_2 0.707106781186547524401f
#define TAU (PI*2.f)
#define GOLDEN_ANGLE 2.39996322972865332f

// bring certain std functions into global scope
template <typename T> inline const T& min(const T& a, const T& b) { return std::min(a, b); }
template <typename T> inline const T& max(const T& a, const T& b) { return std::max(a, b); }
template <typename T> inline const T& abs(const T& a) { return std::abs(a); }
template <typename T> inline const T& sqrt(const T& a) { return std::sqrt(a); }


/* Real and Integer functions */

template<typename T>
inline T clamp(T s, T s_min, T s_max) {
	return std::max(s_min, std::min(s_max, s));
}

template<typename T>
inline T sq(T a) {
	return a * a;
}


/* Integer Functions */

template <typename T>
inline T sign(T a) {
	if (a > 0) return 1;
	else if (a < 0) return -1;
	else return 0;
}


/* Real Functions */

template <typename T>
inline T fract(T f) { return f - std::floor(f);  }

template <typename T>
inline T lerp(T a, T b, T t) { return (b-a)*t + a;  }

inline float sign(float a) {
	if (a > 0.0f) return 1.0f;
	else if (a < 0.0f) return -1.0f;
	else return 0.0f;
}
inline double sign(double a) {
	if (a > 0.0) return 1.0;
	else if (a < 0.0) return -1.0;
	else return 0.0;
}
