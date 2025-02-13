#pragma once
#include "maths.h"

struct sides2 {
    vec2 min; // min x and y sides
    vec2 max; // max x and y sides
    void flipX(); // flip so that min x is applied to max x of the box
    void flipY(); 
    vec2 total(); // sum total of both sides on each axis
};

struct box2 {
    vec2 min, max;
    box2() { }
    box2(vec2 _min, vec2 _max) : min(_min), max(_max) { }

    static box2 fromCenterHalfExt(vec2 center, vec2 half_ext);
    static box2 fromCornerFullExt(vec2 min_corner, vec2 full_ext);
    static box2 fromFltMax();

    // Does this box contain the point p within it's [min,max] range.
    bool contains(vec2 p) const;

    // The half extent of this box.
    vec2 halfExt() const;

    // The full extent of this box.
    vec2 fullExt() const;

    // The center of this box.
    vec2 center() const;

    // Grow this box so that it fully contains another box.
    void growToContain(box2 box);

    // Enlarge this box by halfext on each side.
    void dilateBy(vec2 halfext);

    void growBy(sides2 s);
    void shrinkBy(sides2 s);
}; 

inline box2 box2::fromCenterHalfExt(vec2 center, vec2 half_ext) {
    return box2(center - half_ext, center + half_ext);
}
inline box2 box2::fromCornerFullExt(vec2 min_corner, vec2 full_ext) {
    return box2(min_corner, min_corner + full_ext);
}
inline box2 box2::fromFltMax() {
    return box2(vec2(+FLT_MAX), vec2(-FLT_MAX));
}
inline bool box2::contains(vec2 p) const {
    return all(p >= min) && all(p <= max); // Can I do all() just once?
}
inline vec2 box2::halfExt() const { return (max-min)*0.5f; }
inline vec2 box2::fullExt() const { return (max-min)     ; }
inline vec2 box2::center()  const { return (max+min)*0.5f; }
inline void box2::growToContain(box2 box) {
    min = ::min(min, box.min);
    max = ::max(max, box.max);
}
inline void box2::dilateBy(vec2 halfext) {
    min -= halfext;
    max += halfext;
}
inline box2 round(box2 b) {
    return box2(round(b.min), round(b.max));
}
// When rounding a box that contains (crosses) zero, the min will be negative and max will be positive.
// Using standard round (which rounds towards zero) will have the effect of shrinking or enlarging the box.
// This round will instead always bias towards zero, which will preserve the dimensions of the box more exactly.
inline box2 roundTowardZero(box2 b) {
    return box2(roundTowardZero(b.min), roundTowardZero(b.max));
}