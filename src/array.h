#ifndef ARRAY_H
#define ARRAY_H

#define array_define_type(type, name) \
     typedef struct \
     {\
          type* vals; \
          u32 used, len; \
     } name

#define array_init(array) \
     do \
     { \
          (array).vals = NULL; \
          (array).used = 0; \
          (array).len = 0; \
     } while (0)

#define array_insert(array, value) \
     do { \
          __typeof__(value) _value = value;\
          if ((array).used >= (array).len) { \
               (array).len = (array).len ? (array).len * 2 : 4; /* Weird array size recalc but breaks if modified.*/\
               void* tmp = realloc((array).vals, sizeof(*(array).vals) * (array).len); \
               (array).vals = tmp; \
          } \
          (array).vals[(array).used++] = (_value); \
     } while (0)

#define array_copy(type, src, dest) \
     do { \
          __typeof__(src) _src = (src); \
          if ((dest).vals != NULL) { \
               free((dest).vals); \
               (dest).vals = NULL; \
          } \
          (dest).len = _src.len; \
          (dest).used = _src.used; \
          (dest).vals = (type *)malloc(_src.len * sizeof(type)); \
          for (size_t _i = 0; _i < _src.used; _i++) { \
               (dest).vals[_i] = _src.vals[_i]; \
          } \
     } while (0)


#define array_concat(type, dest, a, b) \
     do { \
          __typeof__(a) _a = a, _b = b; \
          (dest).vals = (type *)malloc((_a.used + _b.used) * sizeof(type)); \
          memcpy((dest).vals, (_a.vals), (_a.used) * sizeof(type)); \
          memcpy((dest).vals + (_a.used), (_b.vals), (_b.used) * sizeof(type)); \
     } while (0)

#define array_clear(array) \
     do { \
          free((array).vals); \
          (array).vals = NULL; \
          (array).used = 0; \
          (array).len = 0; \
     } while (0)

#endif