#pragma once
#include "maths.h"

/* Notes:
   a c-style pose2_I() pose2_T(x,y) api may be easier type actually..
   and it would cross compile to gpu too...
*/

struct pose2 {
	vec2 p;
	vec2 rs;

	static pose2 I();
	static pose2 T(float, float);
	static pose2 T(vec2);
	static pose2 R(float);
	static pose2 R(vec2);
	static pose2 S(float);
	static pose2 RS(float, float);
	static pose2 RS(vec2, float);
	static pose2 TS(vec2, float);
	static pose2 TR(vec2, float);
	static pose2 TR(vec2, vec2);
	static pose2 TRS(vec2, float, float);
	static pose2 TRS(vec2, vec2, float);

	vec2  getP() const;
	vec2  getRS() const;
	float getS() const;
};

inline pose2 pose2::I()        { return pose2{vec2(0.0f), vec2(1.0f, 0.0f)}; }
inline pose2 pose2::T(float x, float y) { return pose2{vec2(x,y), vec2(1.0f, 0.0f)}; }
inline pose2 pose2::T(vec2 v)  { return pose2{v, vec2(1.0f, 0.0f)}; }
inline pose2 pose2::R(float r) { return pose2{vec2(0.0f), vec2(cosf(r), sinf(r))}; }
inline pose2 pose2::R(vec2 rv) { return pose2{vec2(0.0f), rv}; }
inline pose2 pose2::S(float s) { return pose2{vec2(0.0f), vec2(s, 0.0f)}; }
inline pose2 pose2::RS(float r, float s) { return pose2{ vec2(0.0f), vec2(cosf(r), sinf(r)) * s }; }
inline pose2 pose2::RS(vec2 rv, float s) { return pose2{ vec2(0.0f), rv * s }; }
inline pose2 pose2::TS(vec2 v, float s) { return pose2{v, vec2(s, 0.0f)}; }
inline pose2 pose2::TR(vec2 v, float r) { return pose2{v, vec2(cosf(r), sinf(r)) }; }
inline pose2 pose2::TR(vec2 v, vec2 rv) { return pose2{v, rv}; }
inline pose2 pose2::TRS(vec2 v, float r, float s) { return pose2{ v, vec2(cosf(r), sinf(r)) * s }; }
inline pose2 pose2::TRS(vec2 v, vec2 rv, float s) { return pose2{ v, rv * s }; }
inline vec2  pose2::getP() const { return p; };
inline vec2  pose2::getRS() const { return rs; }
inline float pose2::getS() const { return length(rs); }
inline pose2 operator*(pose2 lhs, pose2 rhs) {
	vec2 p  = lhs.p + rotate(lhs.rs, rhs.p);
	vec2 rs = rotate(lhs.rs, rhs.rs);
	return pose2{p, rs};
}
inline vec2  operator*(pose2 lhs, vec2 rhs) { return lhs.p + rotate(lhs.rs, rhs); }
inline pose2 inverse(pose2 uno) {
	float s = lengthsq(uno.rs); // #WTF?? is this because composing rotations is done via rotate() and that just muls the components, which ends up making the scale^2 or some such?
	return pose2{vec2(0.0f), vec2(uno.rs.x / s, -uno.rs.y / s)} * pose2{-uno.p, vec2(1.0f, 0.0f)};
}
inline pose2 operator~(pose2 uno) {
	return inverse(uno);
}