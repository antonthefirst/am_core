#pragma once
#include "maths.h"

struct quat {
	float x, y, z, w;

	quat() { }
	quat(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) { }

	static quat I();
	static quat fromAxisAngle(vec3 axis, float angle);
	static quat fromCols(vec3 col0, vec3 col1, vec3 col2);
};

inline quat quat::I() {
	return quat{0.f, 0.f, 0.f, 1.f};
}
inline quat quat::fromAxisAngle(vec3 axis, float angle) {
	vec3 r = axis * sinf(angle * 0.5f);
	float i = cosf(angle * 0.5f);
	return quat{ r.x, r.y, r.z, i };
}


inline quat conj(quat q) {
	return quat{-q.x, -q.y, -q.z, q.w};
}

inline quat lerp(quat a, quat b, float t) {
	return quat { lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t), lerp(a.w, b.w, t) }; 
}

inline float length(quat q) {
	return length(vec4(q.x, q.y, q.z, q.w));
}
inline quat normalize(quat q) {
	float l = length(q);
	return quat { q.x / l, q.y / l, q.z / l, q.w / l};
}
inline float dot(quat a, quat b) {
	return dot(vec4(a.x, a.y, a.z, a.w), vec4(b.x, b.y, b.z, b.w));
}
inline quat inverse(quat q) {
	quat  c = conj(q);
	float l = length(q);
	return quat { c.x / l, c.y / l, c.z / l, c.w / l };
}

inline quat operator*(quat lhs, quat rhs) {
	return quat{
		lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
		lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
		lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
		lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
	};
}

inline vec3 xfmVec(quat q, vec3 v) {
	quat temp = q * quat{v.x, v.y, v.z, 0};
	temp = temp * conj(q);
	return vec3(temp.x, temp.y, temp.z);
}

inline ivec3 xfmVec(quat q, ivec3 v) {
	return round(xfmVec(q, vec3(v)));
}

// Optimize later, since lots of entries should be zero.
inline vec3 xfmX(quat q) {
	return xfmVec(q, vec3(1.0f, 0.0f, 0.0f));
}
inline vec3 xfmY(quat q) {
	return xfmVec(q, vec3(0.0f, 1.0f, 0.0f));
}
inline vec3 xfmZ(quat q) {
	return xfmVec(q, vec3(0.0f, 0.0f, 1.0f));
}

inline vec3 xfmNor(quat q, vec3 v) {
	// Same as xfmVec, cause no scale encoded in quat.
	return xfmVec(q, v);
}
inline ivec3 xfmNor(quat q, ivec3 v) {
	// Same as xfmVec, cause no scale encoded in quat.
	return xfmVec(q, v);
}

// From vectormath lib
inline quat quat_from_mat3x3(vec3 col0, vec3 col1, vec3 col2) {
	float trace, radicand, scale, xx, yx, zx, xy, yy, zy, xz, yz, zz, tmpx, tmpy, tmpz, tmpw, qx, qy, qz, qw;
	int negTrace, ZgtX, ZgtY, YgtX;
	int largestXorY, largestYorZ, largestZorX;

	xx = col0.x;
	yx = col0.y;
	zx = col0.z;
	xy = col1.x;
	yy = col1.y;
	zy = col1.z;
	xz = col2.x;
	yz = col2.y;
	zz = col2.z;

	trace = ( ( xx + yy ) + zz );

	negTrace = ( trace < 0.0f );
	ZgtX = zz > xx;
	ZgtY = zz > yy;
	YgtX = yy > xx;
	largestXorY = ( !ZgtX || !ZgtY ) && negTrace;
	largestYorZ = ( YgtX || ZgtX ) && negTrace;
	largestZorX = ( ZgtY || !YgtX ) && negTrace;

	if ( largestXorY )
	{
		zz = -zz;
		xy = -xy;
	}
	if ( largestYorZ )
	{
		xx = -xx;
		yz = -yz;
	}
	if ( largestZorX )
	{
		yy = -yy;
		zx = -zx;
	}

	radicand = ( ( ( xx + yy ) + zz ) + 1.0f );
	scale = ( 0.5f * ( 1.0f / sqrt( radicand ) ) );

	tmpx = ( ( zy - yz ) * scale );
	tmpy = ( ( xz - zx ) * scale );
	tmpz = ( ( yx - xy ) * scale );
	tmpw = ( radicand * scale );
	qx = tmpx;
	qy = tmpy;
	qz = tmpz;
	qw = tmpw;

	if ( largestXorY )
	{
		qx = tmpw;
		qy = tmpz;
		qz = tmpy;
		qw = tmpx;
	}
	if ( largestYorZ )
	{
		tmpx = qx;
		tmpz = qz;
		qx = qy;
		qy = tmpx;
		qz = qw;
		qw = tmpz;
	}

	quat ret;
	ret.x = qx;
	ret.y = qy;
	ret.z = qz;
	ret.w = qw;
	return ret;
}

inline quat quat::fromCols(vec3 col0, vec3 col1, vec3 col2) {
	return quat_from_mat3x3(col0, col1, col2);
}

// Return one of [0,23] 90 degree rotations.
// 0 is identity.
// Arguments are columns of a 3x3 matrix.
inline void get90Rotation(int i, vec3* x, vec3* y, vec3* z) {
	static vec3 rots[24][3] = {
		// For each way Z can face, rotate around it 4 times. 6*4 = 24
		// Z = 0,0,+1
		{{+1.0f,  0.0f,  0.0f}, { 0.0f, +1.0f,  0.0f}, { 0.0f,  0.0f, +1.0f}}, // identity
		{{ 0.0f, +1.0f,  0.0f}, {-1.0f,  0.0f,  0.0f}, { 0.0f,  0.0f, +1.0f}},
		{{-1.0f,  0.0f,  0.0f}, { 0.0f, -1.0f,  0.0f}, { 0.0f,  0.0f, +1.0f}},
		{{ 0.0f, -1.0f,  0.0f}, {+1.0f,  0.0f,  0.0f}, { 0.0f,  0.0f, +1.0f}},

		// Z = 0,0,-1
		{{+1.0f,  0.0f,  0.0f}, { 0.0f, -1.0f,  0.0f}, { 0.0f,  0.0f, -1.0f}},
		{{ 0.0f, -1.0f,  0.0f}, {-1.0f,  0.0f,  0.0f}, { 0.0f,  0.0f, -1.0f}},
		{{-1.0f,  0.0f,  0.0f}, { 0.0f, +1.0f,  0.0f}, { 0.0f,  0.0f, -1.0f}},
		{{ 0.0f, +1.0f,  0.0f}, {+1.0f,  0.0f,  0.0f}, { 0.0f,  0.0f, -1.0f}},

		// Z = 0,+1,0
		{{+1.0f,  0.0f,  0.0f}, { 0.0f,  0.0f, -1.0f}, { 0.0f, +1.0f,  0.0f}},
		{{ 0.0f,  0.0f, -1.0f}, {-1.0f,  0.0f,  0.0f}, { 0.0f, +1.0f,  0.0f}},
		{{-1.0f,  0.0f,  0.0f}, { 0.0f,  0.0f, +1.0f}, { 0.0f, +1.0f,  0.0f}},
		{{ 0.0f,  0.0f, +1.0f}, {+1.0f,  0.0f,  0.0f}, { 0.0f, +1.0f,  0.0f}},

		// Z = 0,-1,0
		{{+1.0f,  0.0f,  0.0f}, { 0.0f,  0.0f, +1.0f}, { 0.0f, -1.0f,  0.0f}},
		{{ 0.0f,  0.0f, +1.0f}, {-1.0f,  0.0f,  0.0f}, { 0.0f, -1.0f,  0.0f}},
		{{-1.0f,  0.0f,  0.0f}, { 0.0f,  0.0f, -1.0f}, { 0.0f, -1.0f,  0.0f}},
		{{ 0.0f,  0.0f, -1.0f}, {+1.0f,  0.0f,  0.0f}, { 0.0f, -1.0f,  0.0f}},

		// Z = +1,0,0
		{{ 0.0f,  0.0f, -1.0f}, { 0.0f, +1.0f,  0.0f}, {+1.0f,  0.0f,  0.0f}},
		{{ 0.0f, +1.0f,  0.0f}, { 0.0f,  0.0f, +1.0f}, {+1.0f,  0.0f,  0.0f}},
		{{ 0.0f,  0.0f, +1.0f}, { 0.0f, -1.0f,  0.0f}, {+1.0f,  0.0f,  0.0f}},
		{{ 0.0f, -1.0f,  0.0f}, { 0.0f,  0.0f, -1.0f}, {+1.0f,  0.0f,  0.0f}},

		// Z = -1,0,0
		{{ 0.0f,  0.0f, +1.0f}, { 0.0f, +1.0f,  0.0f}, {-1.0f,  0.0f,  0.0f}},
		{{ 0.0f, +1.0f,  0.0f}, { 0.0f,  0.0f, -1.0f}, {-1.0f,  0.0f,  0.0f}},
		{{ 0.0f,  0.0f, -1.0f}, { 0.0f, -1.0f,  0.0f}, {-1.0f,  0.0f,  0.0f}},
		{{ 0.0f, -1.0f,  0.0f}, { 0.0f,  0.0f, +1.0f}, {-1.0f,  0.0f,  0.0f}},
	};
	*x = rots[i][0];
	*y = rots[i][1];
	*z = rots[i][2];
}
inline quat get90Quat(int i) {
	vec3 x,y,z;
	get90Rotation(i, &x, &y, &z);
	return quat::fromCols(x,y,z);
}

// Return [0,23] for each corresponding 90 degree rotation. No flips. 
// Rotation must be near 90 degree, otherwise the result is identity.
// (snap the rotation to 90 first, if it's not)
inline uint8_t get90RotationIdx(quat q) {
	vec3 qx = round(xfmX(q));
	vec3 qy = round(xfmY(q));
	vec3 qz = round(xfmZ(q));
	for (uint8_t i = 0; i < 24; ++i) {
		vec3 x,y,z;
		get90Rotation(i, &x, &y, &z);
		if (all(x == qx) && all(y == qy) && all(z == qz)) {
			return i;
		}
	}
	// Should never happen.
	return 0;
}

// Snap the quaternion to divisions of D
inline quat snapQuat(quat ang, int d) {
	float best_align = 0;
	quat best = ang;
	for(int elev = 0; elev <= d / 2; ++elev) {
		for(int azim = 0; azim < d; ++azim) {
			for(int r = 0; r < d; ++r) {
				// Rotate axis into place
				quat aroundY = quat::fromAxisAngle(vec3(0.0f, 1.0f, 0.0f), azim / float(d) * 2.f * PI);
				quat aroundX = quat::fromAxisAngle(vec3(1.0f, 0.0f, 0.0f), elev / float(d / 2) * PI);
				quat q = aroundY * aroundX;
				// R rotations around the axis
				q = q * quat::fromAxisAngle(vec3(0.0f, 1.0f, 0.0f), r / float(d) * 2.f * PI);
				float align = saturate(abs(dot(q, ang)));
				if(align > best_align) {
					best_align = align;
					best = q;
				}
			}
		}
	}

	return best;
}

inline quat snapQuatTo90(quat ang) {
	return snapQuat(ang, 4);
}


#if 0
inline vec4 quat_identity() { return vec4(0.f, 0.f, 0.f, 1.f); }
inline vec4 quat_mul(vec4 q1, vec4 q2) {
	return vec4(
		q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
		q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
		q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
		q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z);
}
inline vec4 quat_conj(vec4 q) {
	return vec4(-q.x, -q.y, -q.z, q.w);
}
inline vec3 quat_mul_vec(vec4 q, vec3 v) {
	vec4 temp = quat_mul(q, vec4(v.x, v.y, v.z, 0));
	temp = quat_mul(temp, quat_conj(q));
	return vec3(temp.x, temp.y, temp.z);
}
inline vec4 quat_rotation_around_axis(vec3 axis, float ang) {
	return vec4(vec3(axis * sinf(ang * 0.5f)), cosf(ang * 0.5f));
}
inline vec4 quat_from_rots(vec3 rot) {
	vec4 qx = vec4(vec3(sinf(rot.x * 0.5f), 0, 0), cosf(rot.x * 0.5f));
	vec4 qy = vec4(vec3(0, sinf(rot.y * 0.5f), 0), cosf(rot.y * 0.5f));
	vec4 qz = vec4(vec3(0, 0, sinf(rot.z * 0.5f)), cosf(rot.z * 0.5f));
	return quat_mul(qz, quat_mul(qy, qx));
}
// Doesn't work if vectors point away from one another.
inline vec4 quat_from_arc(vec3 from, vec3 to)
{
	// from: https://stackoverflow.com/questions/1171849/finding-quaternion-representing-the-rotation-from-one-vector-to-another
	// Overall doesn't seem terribly robust..
	vec4 q;
	q.xyz(cross(from, to));
	q.w = sqrt(lengthsq(from) * lengthsq(to)) + dot(from, to);
	return normalize(q);
}
#endif

/*
vec4 quat_identity() { return vec4(0.f, 0.f, 0.f, 1.f); }
vec4 quat_mul(vec4 q1, vec4 q2) {
return vec4(
q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y,
q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x,
q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w,
q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z);
}
vec4 quat_conj(vec4 q) {
return vec4(-q.x, -q.y, -q.z, q.w);
}
vec3 quat_mul_vec(vec4 q, vec3 v) {
vec4 temp = quat_mul(q, vec4(v.x, v.y, v.z, 0));
temp = quat_mul(temp, quat_conj(q));
return vec3(temp.x, temp.y, temp.z);
}
vec4 quat_rotation_around_axis(vec3 axis, float ang) {
return vec4(vec3(axis * sinf(ang * 0.5f)), cosf(ang * 0.5f));
}
vec4 quat_from_rots(vec3 rot) {
vec4 qx = vec4(vec3(sinf(rot.x * 0.5f), 0, 0), cosf(rot.x * 0.5f));
vec4 qy = vec4(vec3(0, sinf(rot.y * 0.5f), 0), cosf(rot.y * 0.5f));
vec4 qz = vec4(vec3(0, 0, sinf(rot.z * 0.5f)), cosf(rot.z * 0.5f));
return quat_mul(qz, quat_mul(qy, qx));
}

*/