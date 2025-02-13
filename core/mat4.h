#pragma once
#include "maths.h"

struct pose3;

struct mat4 {
	float entries[16];
	mat4() { }
	explicit mat4(pose3& p);

	static mat4 fromPose(const pose3& p);
	static mat4 fromCols(vec4 col0, vec4 col1, vec4 col2, vec4 col3);

	float& operator[](int i) { return entries[i]; }
};

mat4 operator*(const mat4& lhs, const mat4& rhs);
mat4 inverse(const mat4& mat);
vec4 xfmVec(const mat4& mat, vec4 vec);

// Right handed, reverse-z, infinite far plane, column major.
mat4 mat4PerspectiveProjection(float half_fov_y_rad, float aspect_ratio_w_by_h,float z_near);

