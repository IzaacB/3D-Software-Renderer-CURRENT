#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <stdbool.h>

typedef struct
{
     float x, y;
} v2;

typedef struct
{
     float x, y, z;
} v3;

typedef struct
{
    v3 position, rotation, scale;
}
transform;

#define v2_add(_a, _b) ({ __typeof__(_a) __a = _a, __b = _b; (v2) {__a.x + __b.x, __a.y + __b.y}; })
#define v2_sub(_a, _b) ({ __typeof__(_a) __a = _a, __b = _b; (v2) {__a.x - __b.x, __a.y - __b.y}; })
#define v2_scale(_a, _s) ({ __typeof__(_a) __a = _a; __typeof__(_s) __s = _s; (v2) {__a.x * __s, __a.y * __s}; })
#define v2_mag(_a) ({ __typeof__(_a) __a = _a; (float) sqrtf(__a.x * __a.x + __a.y * __a.y); })
#define v2_norm(_a) ({ __typeof__(_a) __a = _a; float _mag = v2_mag(_a); (v2) {__a.x / _mag, __a.y / _mag}; })
#define v2_dot(_a, _b) ({ __typeof__(_a) __a = _a, __b = _b; (float) (__a.x * __b.x + __a.y * __b.y); })

#define v3_to_v2(_a) ({__typeof__(_a) __a = _a; (v2) {__a.x, __a.y}; })
#define v3_add(_a, _b) ({ __typeof__(_a) __a = _a, __b = _b; (v3) {__a.x + __b.x, __a.y + __b.y, __a.z + __b.z}; })
#define v3_sub(_a, _b) ({ __typeof__(_a) __a = _a, __b = _b; (v3) {__a.x - __b.x, __a.y - __b.y, __a.z - __b.z}; })
#define v3_scale(_a, _s) ({ __typeof__(_a) __a = _a; __typeof__(_s) __s = _s; (v3) {__a.x * __s, __a.y * __s, __a.z * __s}; })
#define v3_mag(_a) ({ __typeof__(_a) __a = _a; (float) sqrtf(__a.x * __a.x + __a.y * __a.y + __a.z * __a.z); })
#define v3_norm(_a) ({ __typeof__(_a) __a = _a; float _mag = v3_mag(_a); (v3) {__a.x / _mag, __a.y / _mag, __a.z / _mag}; })
#define v3_dot(_a, _b) ({ __typeof__(_a) __a = _a, __b = _b; (float) (__a.x * __b.x + __a.y * __b.y + __a.z * __b.z); })
#define v3_cross(_a, _b) ({ __typeof__(_a) __a = _a, __b = _b; (v3) {__a.y * __b.z - __a.z * __b.y, __a.z * __b.x - __a.x * __b.z, __a.x * __b.y - __a.y * __b.x}; })

extern v3 v3_transform(v3 v, transform t, unsigned int mode);

#endif
