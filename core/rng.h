#pragma once
#include "core/basic_types.h"

/* Notes:

- uniform floats/doubles are not *exactly* uniform, but one of 2^24 unique values (which is the common approximation).

*/

struct rng32 {
	u32 s[2];

	rng32() { /*** UNINITIALIZED ***/ }
	rng32(u32 seed);
	void seed(u32 seed);

	// raw
	u32 bits();

	// floats
	float  unit(); // [0,1]
	float eunit(); // [0,1)
	float  snit(); // [-1,+1]
	float esnit(); // (-1,+1)
	float  unit(float s); // [0,s]
	float eunit(float s); // [0,s)
	float  snit(float s); // [-s,+s]
	float esnit(float s); // (-s,+s)

	// integer uniform distribution
	int operator()(int min, int max); // [min,max]
	int operator()(         int max); // [0,  max]

	// floating point uniform distribution
	float  operator()();                       // [0,1]
	float  operator()( float min,  float max); // [min,max]
	float  operator()(             float max); // [0,  max]
	double operator()(double min, double max); // [min,max]
	double operator()(            double max); // [0,  max]

	// chances
	bool one_in(int count);            // true one in COUNT times. COUNT must be > 0

	// triangle-shaped distribution
	float     tri(float min, float max); // [min,max]
	float tri_min(float min, float max); // [min,max], aligned to min
	float tri_max(float min, float max); // [min,max], aligned to max

	// bell-shaped distribution
	float     bell(float min, float max); // [min,max]
	float bell_min(float min, float max); // [min,max], aligned to min
	float bell_max(float min, float max); // [min,max], aligned to max

	// spike-shaped distribution
	float     spike(float min, float max); // [min,max]
	float spike_min(float min, float max); // [min,max], aligned to min
	float spike_max(float min, float max); // [min,max], aligned to max
};

struct rng64 {

};

struct rng128 {

};

/* consider:

namespace grng { // g because otherwise can't have a variable called rng which sucks :(
	//internally an rng64, has same api, to provide 32, and 64 bit nums
};

*/
