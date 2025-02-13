#include "color.h"
#include "colorspace.h"

vec3 hsv2rgb(vec3 hsv) {
    vec3 ret;
    Hsv2Rgb(&ret.x,&ret.y,&ret.z, hsv.x*360.0f, hsv.y, hsv.z);
    return ret;
}


col3 fromHSV(float h, float s, float v) {
    col3 ret;
    Hsv2Rgb(&ret.r,&ret.g,&ret.b, h*360.0f, s, v);
    return ret;
}
void toHSV(col3 col, float* h, float* s, float* v) {
    Rgb2Hsv(h, s, v, col.r, col.g, col.b);
    *h /= 360.f;
}

#if 0
inline float cheap_saturation(vec3 c)
{
    float mi = min(min(c.x, c.y), c.z);
    float ma = max(max(c.x, c.y), c.z);
    return (ma - mi)/(ma+ 1e-7);
}

inline vec3 resaturate_after_lerp(vec3 base_lerp, float target_saturation) {
    //Interpolated base color with singularity fix
    vec3 ic = base_lerp + vec3(1e-6,0.,0.);

    //Saturation difference from ideal scenario
    float sd = abs(cheap_saturation(ic) - target_saturation);

    //Displacement direction
    vec3 dir = normalize(vec3(2.f*ic.x - ic.y - ic.z, 2.*ic.y - ic.x - ic.z, 2.*ic.z - ic.y - ic.x));
    //Simple Lighntess
    float lgt = dot(vec3(1.f,1.f,1.f), ic);

    //Extra scaling factor for the displacement
    float ff = dot(dir, normalize(ic));

    //Displace the color
#define DSP_STR 1.5f
    ic += dir*DSP_STR*sd*ff*lgt;
    return clampvec(ic,vec3(0.f),vec3(1.f));
}

#endif