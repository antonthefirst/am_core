#pragma once
#include "maths.h"

//#TODO put into generator
inline vec2 floor(vec2 v) {
	return vec2(floorf(v.x), floorf(v.y));
}
// Round away from zero, towards the nearest integer.
inline vec2 round(vec2 v) {
	return vec2(roundf(v.x), roundf(v.y));
}
inline vec2 roundTowardZero(vec2 v) {
	return vec2(roundTowardZero(v.x), roundTowardZero(v.y));
}
inline vec3 floor(vec3 v) {
	return vec3(floorf(v.x), floorf(v.y), floorf(v.z));
}
inline float step(float edge, float v) {
	return v < edge ? 0.0f : 1.0f;
}
inline vec3 step(vec3 edge, vec3 v) {
	return vec3(step(edge.x, v.x), step(edge.y, v.y), step(edge.z, v.z));
}

inline vec3 cross(vec3 a, vec3 b) {
	return vec3(a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y);
}
inline dvec3 cross(dvec3 a, dvec3 b) {
	return dvec3(a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y);
}
inline int minidx(vec3 v) {
	if (v.x < v.y) {
		if (v.x < v.z) {
			return 0;
		} else {
			return 2;
		}
	} else {
		if (v.y < v.z) {
			return 1;
		} else {
			return 2;
		}
	}
}
inline int maxidx(vec3 v) {
	if (v.x > v.y) {
		if (v.x > v.z) {
			return 0;
		} else {
			return 2;
		}
	} else {
		if (v.y > v.z) {
			return 1;
		} else {
			return 2;
		}
	}
}
