#pragma once
#include "maths.h"
#include "quat.h"

struct pose3 {
	vec3 pos; float scale;
	quat rot;
	pose3() { }
	pose3(vec3 _pos, float _scale, quat _rot) : pos(_pos), scale(_scale), rot(_rot) { }
	pose3 operator~() const;
	vec3 operator*(vec3 rhs) const;

	static pose3 I();
	static pose3 T(vec3 pos);
	static pose3 R(quat rot);
	static pose3 S(float scale);
	static pose3 TR(vec3 pos, quat rot);
	static pose3 RS(quat rot, float scale);
};

inline pose3 pose3::I()                      { return pose3(vec3(0.0f), 1.0f, quat::I()); }
inline pose3 pose3::T(vec3 _pos)             { return pose3(_pos,       1.0f, quat::I()); }
inline pose3 pose3::R(quat _rot)             { return pose3(vec3(0.0f), 1.0f, _rot); }
inline pose3 pose3::S(float _scale)          { return pose3(vec3(0.0f), _scale, quat::I()); }
inline pose3 pose3::TR(vec3 _pos, quat _rot) { return pose3(_pos,       1.0f, _rot); }
inline pose3 pose3::RS(quat _rot, float _scale) { return pose3(vec3(0.0f), _scale, _rot); }

inline pose3 inverse(pose3 p) {
	pose3 res;
	res.rot = inverse(p.rot);
	res.scale = 1.0f / p.scale;
	res.pos = xfmVec(res.rot, -p.pos * res.scale);
	return res;
}

inline pose3 pose3::operator~() const {
	return inverse(*this);
}

inline vec3 xfmVec(pose3 lhs, vec3 rhs) {
	return lhs.pos + xfmVec(lhs.rot, rhs) * lhs.scale;
}

inline vec3 xfmNor(pose3 lhs, vec3 rhs) {
	// Don't apply scale. However, this also prevents flips, which we may want to bring back...
	return xfmVec(lhs.rot, rhs);
}

inline pose3 operator*(pose3 lhs, pose3 rhs) {
	return pose3(xfmVec(lhs.rot, rhs.pos) * lhs.scale + lhs.pos, lhs.scale * rhs.scale, lhs.rot * rhs.rot);
}

inline pose3 nlerp(pose3 a, pose3 b, float t) {
	return pose3(lerp(a.pos, b.pos, t), lerp(a.scale, b.scale, t), normalize(lerp(a.rot, b.rot, t)));
}
