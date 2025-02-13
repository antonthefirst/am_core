#pragma once
#include "col.h"

// Maybe these ultimately become "constructors" inside col3.h.
col3 fromHSV(float h, float s, float v);
void toHSV(col3 col, float* h, float* s, float* v);
