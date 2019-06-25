// 32-bit rng from: http://xoshiro.di.unimi.it/xoroshiro64starstar.c
// 32-bit seed initialization from: https://stackoverflow.com/questions/17035441/looking-for-decent-quality-prng-with-only-32-bits-of-state

#include "rng.h"
#include <float.h> // for FLT_MANT_DIG
#include "maths.h" // for copysign
#include "log.h"   // for assert

static inline u32 splitmix32(u32 b) {
	b += 0x9e3779b9;
	b ^= b >> 15;
	b *= 0x85ebca6b;
	b ^= b >> 13;
	b *= 0xc2b2ae3d;
	b ^= b >> 16;
	return b;
}

rng32::rng32(u32 _seed) {
	seed(_seed);
}
void rng32::seed(u32 _seed) {
	// s[0] and s[1] cannot both be 0, so we do hash twice to ensure that (because it is uniform and thus can't hit 0 twice)
	s[1] = splitmix32(_seed);
	s[0] = splitmix32(s[1]);
}
static inline u32 rotl(const u32 x, int k) { return (x << k) | (x >> (32 - k)); }
u32 rng32::bits() {
	const u32 s0 = s[0];
	u32 s1 = s[1];
	const u32 result_starstar = rotl(s0 * 0x9E3779BB, 5) * 5;
	s1 ^= s0;
	s[0] = rotl(s0, 26) ^ s1 ^ (s1 << 9); // a, b
	s[1] = rotl(s1, 13); // c
	return result_starstar;
}
static inline float unit_from_bits(u32 bits) {
	return (bits >> (32 - FLT_MANT_DIG)) / float((1 << FLT_MANT_DIG) - 1); // #DRAGONS divide is required here; "times_one_over" optimization yields slight excess of 1.0 values! (512 of ~16M instead of 256)
}
static inline float eunit_from_bits(u32 bits) {
	return (bits >> (32 - FLT_MANT_DIG)) * (1.0f /  float((1 << FLT_MANT_DIG))); // #DRAGONS unlike the [0,1] case, the "times_one_over" optimization yields identical results, so we can apply it here
}
float rng32::unit() {
	return  unit_from_bits(bits());
}
float rng32::eunit() {
	return eunit_from_bits(bits());
}
float rng32::snit() {
	u32 b = bits();
	return  copysign(unit_from_bits(b), b & 0x8000000 ? +1.0f : -1.0f);
}
float rng32::esnit() {
	u32 b = bits();
	return copysign(eunit_from_bits(b), b & 0x8000000 ? +1.0f : -1.0f);
}
float rng32::unit (float s) { return  unit()*s; }
float rng32::eunit(float s) { return eunit()*s; }
float rng32::snit (float s) { return  snit()*s; }
float rng32::esnit(float s) { return esnit()*s; }

int rng32::operator()(int min, int max) {
	return min + operator()(max-min);
}
int rng32::operator()(int max) {
	// this code is adapted from: http://www.pcg-random.org, basic generator C file (http://www.pcg-random.org/downloads/pcg-c-basic-0.9.zip)
	// the loop is guaranteed to terminate if the generator is uniform, and in this particular use certainly because the argument is int which is half the range of the underlying u32
	// the chance of this loop re-rolling is roughly proportional to the range
	// for small ranges up to say 1024, the chance is around 0.00002%, or 1 in 4 million.
	// for medium ranges of say 2^16, the changes is around 0.002% or 1 in 66 thousand.
	// so it's safe to say for "most" cases, it will return without actually looping.
	
	u32 bound = max+1;
    uint32_t threshold = -bound % bound;

    // Uniformity guarantees that this loop will terminate
    for (;;) {
        uint32_t r = bits();
        if (r >= threshold)
            return r % bound;
    }
}
float rng32::operator()() { return unit(); }
float rng32::operator()(float min, float max) { return lerp(min, max, unit()); }
float rng32::operator()(float max) { return unit(max); }
double rng32::operator()(double min, double max) { return lerp(min, max, double(unit())); }
double rng32::operator()(double max) { return double(unit(max)); }

bool rng32::one_in(int count) {
	assert(count != 0);
	return operator()(count-1) == 0;
}
