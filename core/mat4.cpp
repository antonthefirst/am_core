#include "mat4.h"
#include "maths.h"
#include "pose3.h"

// Mathematical notation is Column Vectors
// Memory layout is Column Major
#define M33(r,c) (*(mat + r + c*3))
#define M44(r,c) (*(mat + r + c*4))

//#SOURCE http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
inline void mat4x4_from_pose(float* mat, vec3 pos, float scale, quat q) {
	float sqw = q.w*q.w;
	float sqx = q.x*q.x;
	float sqy = q.y*q.y;
	float sqz = q.z*q.z;

	// invs (inverse square length) is only required if quaternion is not already normalised
	float invs = 1.f / (sqx + sqy + sqz + sqw);
	M44(0,0) = (sqx - sqy - sqz + sqw)*invs*scale; // since sqw + sqx + sqy + sqz =1/invs*invs
	M44(1,1) = (-sqx + sqy - sqz + sqw)*invs*scale;
	M44(2,2) = (-sqx - sqy + sqz + sqw)*invs*scale;

	float tmp1 = q.x*q.y;
	float tmp2 = q.z*q.w;
	M44(1,0) = 2.f * (tmp1 + tmp2)*invs;
	M44(0,1) = 2.f * (tmp1 - tmp2)*invs;

	tmp1 = q.x*q.z;
	tmp2 = q.y*q.w;
	M44(2,0) = 2.f * (tmp1 - tmp2)*invs;
	M44(0,2) = 2.f * (tmp1 + tmp2)*invs;
	tmp1 = q.y*q.z;
	tmp2 = q.x*q.w;
	M44(2,1) = 2.f * (tmp1 + tmp2)*invs;
	M44(1,2) = 2.f * (tmp1 - tmp2)*invs;

	M44(0, 3) = pos.x;
	M44(1, 3) = pos.y;
	M44(2, 3) = pos.z;
	M44(3, 3) = 1.f;

	M44(3, 0) = 0.f;
	M44(3, 1) = 0.f;
	M44(3, 2) = 0.f;
}

//#SOURCE http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
mat4 mat4Pose(vec3 pos, float scale, vec4 q) {
	mat4 mat_ret;
	float* mat = (float*)&mat_ret;
	float sqw = q.w*q.w;
	float sqx = q.x*q.x;
	float sqy = q.y*q.y;
	float sqz = q.z*q.z;

	// invs (inverse square length) is only required if quaternion is not already normalized
	float invs = 1.f / (sqx + sqy + sqz + sqw);
	M44(0,0) = (sqx - sqy - sqz + sqw)*invs*scale; // since sqw + sqx + sqy + sqz =1/invs*invs
	M44(1,1) = (-sqx + sqy - sqz + sqw)*invs*scale;
	M44(2,2) = (-sqx - sqy + sqz + sqw)*invs*scale;

	float tmp1 = q.x*q.y;
	float tmp2 = q.z*q.w;
	M44(1,0) = 2.f * (tmp1 + tmp2)*invs;
	M44(0,1) = 2.f * (tmp1 - tmp2)*invs;

	tmp1 = q.x*q.z;
	tmp2 = q.y*q.w;
	M44(2,0) = 2.f * (tmp1 - tmp2)*invs;
	M44(0,2) = 2.f * (tmp1 + tmp2)*invs;
	tmp1 = q.y*q.z;
	tmp2 = q.x*q.w;
	M44(2,1) = 2.f * (tmp1 + tmp2)*invs;
	M44(1,2) = 2.f * (tmp1 - tmp2)*invs;

	M44(0, 3) = pos.x;
	M44(1, 3) = pos.y;
	M44(2, 3) = pos.z;
	M44(3, 3) = 1.f;

	M44(3, 0) = 0.f;
	M44(3, 1) = 0.f;
	M44(3, 2) = 0.f;
	return mat_ret;
}


// Right handed, reverse-z, infinite far plane, column major.
// from: https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/
mat4 mat4PerspectiveProjection(float half_fov_y_rad, float asp, float z_near) {
	float f = 1.0f / tan(half_fov_y_rad);
	mat4 m;
	m[0] = f / asp;  m[4] = 0;  m[8]  = 0;      m[12] = 0;
	m[1] = 0;        m[5] = f;  m[9]  = 0;      m[13] = 0;
	m[2] = 0;        m[6] = 0;  m[10] = 0;      m[14] = z_near;
	m[3] = 0;        m[7] = 0;  m[11] = -1.0f;  m[15] = 0;
	return m;
}

static void mat_mul(float* r, const float* a, const float* b) {
	r[0]  = a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3];  r[1] = a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3];  r[2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3];  r[3] = a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3];
	r[4]  = a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7];  r[5] = a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7];  r[6] = a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7];  r[7] = a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7];
	r[8]  = a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11];  r[9] = a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11];  r[10] = a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11];  r[11] = a[3] * b[8] + a[7] * b[9] + a[11] * b[10] + a[15] * b[11];
	r[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15];  r[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15];  r[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];  r[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];
}


mat4::mat4(pose3& p) {
	*this = mat4Pose(p.pos, p.scale, vec4(p.rot.x, p.rot.y, p.rot.z, p.rot.w));
}

mat4 mat4::fromPose(const pose3& p) {
	mat4 ret;
	mat4x4_from_pose((float*)&ret, p.pos, p.scale, p.rot);
	return ret;
}
mat4 mat4::fromCols(vec4 col0, vec4 col1, vec4 col2, vec4 col3) {
	mat4 ret;

	// Column major
	ret.entries[0]  = col0[0];
	ret.entries[1]  = col0[1];
	ret.entries[2]  = col0[2];
	ret.entries[3]  = col0[3];

	ret.entries[4]  = col1[0];
	ret.entries[5]  = col1[1];
	ret.entries[6]  = col1[2];
	ret.entries[7]  = col1[3];

	ret.entries[8]  = col2[0];
	ret.entries[9]  = col2[1];
	ret.entries[10] = col2[2];
	ret.entries[11] = col2[3];

	ret.entries[12] = col3[0];
	ret.entries[13] = col3[1];
	ret.entries[14] = col3[2];
	ret.entries[15] = col3[3];

	return ret;
}
mat4 operator*(const mat4& lhs, const mat4& rhs) {
	mat4 res;
	mat_mul(res.entries, lhs.entries, rhs.entries);
	return res;
}

mat4 inverse(const mat4& matrix) {
	const float* mat = (const float*)&matrix;
	vec4 res0, res1, res2, res3;
	float mA, mB, mC, mD, mE, mF, mG, mH, mI, mJ, mK, mL, mM, mN, mO, mP, tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, detInv;
	mA = M44(0, 0);
	mB = M44(1, 0);
	mC = M44(2, 0);
	mD = M44(3, 0);
	mE = M44(0, 1);
	mF = M44(1, 1);
	mG = M44(2, 1);
	mH = M44(3, 1);
	mI = M44(0, 2);
	mJ = M44(1, 2);
	mK = M44(2, 2);
	mL = M44(3, 2);
	mM = M44(0, 3);
	mN = M44(1, 3);
	mO = M44(2, 3);
	mP = M44(3, 3);
	tmp0 = ( ( mK * mD ) - ( mC * mL ) );
	tmp1 = ( ( mO * mH ) - ( mG * mP ) );
	tmp2 = ( ( mB * mK ) - ( mJ * mC ) );
	tmp3 = ( ( mF * mO ) - ( mN * mG ) );
	tmp4 = ( ( mJ * mD ) - ( mB * mL ) );
	tmp5 = ( ( mN * mH ) - ( mF * mP ) );
	res0.x = ( ( ( mJ * tmp1 ) - ( mL * tmp3 ) ) - ( mK * tmp5 ) );
	res0.y = ( ( ( mN * tmp0 ) - ( mP * tmp2 ) ) - ( mO * tmp4 ) );
	res0.z = ( ( ( mD * tmp3 ) + ( mC * tmp5 ) ) - ( mB * tmp1 ) );
	res0.w = ( ( ( mH * tmp2 ) + ( mG * tmp4 ) ) - ( mF * tmp0 ) );
	detInv = ( 1.0f / ( ( ( ( mA * res0.x ) + ( mE * res0.y ) ) + ( mI * res0.x ) ) + ( mM * res0.w ) ) );
	res1.x = ( mI * tmp1 );
	res1.y = ( mM * tmp0 );
	res1.z = ( mA * tmp1 );
	res1.w = ( mE * tmp0 );
	res3.x = ( mI * tmp3 );
	res3.y = ( mM * tmp2 );
	res3.z = ( mA * tmp3 );
	res3.w = ( mE * tmp2 );
	res2.x = ( mI * tmp5 );
	res2.y = ( mM * tmp4 );
	res2.z = ( mA * tmp5 );
	res2.w = ( mE * tmp4 );
	tmp0 = ( ( mI * mB ) - ( mA * mJ ) );
	tmp1 = ( ( mM * mF ) - ( mE * mN ) );
	tmp2 = ( ( mI * mD ) - ( mA * mL ) );
	tmp3 = ( ( mM * mH ) - ( mE * mP ) );
	tmp4 = ( ( mI * mC ) - ( mA * mK ) );
	tmp5 = ( ( mM * mG ) - ( mE * mO ) );
	res2.x = ( ( ( mL * tmp1 ) - ( mJ * tmp3 ) ) + res2.x );
	res2.y = ( ( ( mP * tmp0 ) - ( mN * tmp2 ) ) + res2.y );
	res2.z = ( ( ( mB * tmp3 ) - ( mD * tmp1 ) ) - res2.z );
	res2.w = ( ( ( mF * tmp2 ) - ( mH * tmp0 ) ) - res2.w );
	res3.x = ( ( ( mJ * tmp5 ) - ( mK * tmp1 ) ) + res3.x );
	res3.y = ( ( ( mN * tmp4 ) - ( mO * tmp0 ) ) + res3.y );
	res3.z = ( ( ( mC * tmp1 ) - ( mB * tmp5 ) ) - res3.z );
	res3.w = ( ( ( mG * tmp0 ) - ( mF * tmp4 ) ) - res3.w );
	res1.x = ( ( ( mK * tmp3 ) - ( mL * tmp5 ) ) - res1.x );
	res1.y = ( ( ( mO * tmp2 ) - ( mP * tmp4 ) ) - res1.y );
	res1.z = ( ( ( mD * tmp5 ) - ( mC * tmp3 ) ) + res1.z );
	res1.w = ( ( ( mH * tmp4 ) - ( mG * tmp2 ) ) + res1.w );
	return mat4::fromCols(
		( res0 * detInv ),
		( res1 * detInv ),
		( res2 * detInv ),
		( res3 * detInv )
	);
}

vec4 xfmVec(const mat4& matrix, vec4 vec) {
	const float* mat = (const float*)&matrix;
	return vec4(
		( ( ( ( M44(0,0) * vec.x ) + ( M44(0,1) * vec.y ) ) + ( M44(0,2) * vec.z ) ) + ( M44(0,3) * vec.w ) ),
		( ( ( ( M44(1,0) * vec.x ) + ( M44(1,1) * vec.y ) ) + ( M44(1,2) * vec.z ) ) + ( M44(1,3) * vec.w ) ),
		( ( ( ( M44(2,0) * vec.x ) + ( M44(2,1) * vec.y ) ) + ( M44(2,2) * vec.z ) ) + ( M44(2,3) * vec.w ) ),
		( ( ( ( M44(3,0) * vec.x ) + ( M44(3,1) * vec.y ) ) + ( M44(3,2) * vec.z ) ) + ( M44(3,3) * vec.w ) )
	);
}
