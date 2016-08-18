#pragma once

#include "features.h"

CT_BEGIN_DECLS

#include <math.h>
#include <stddef.h>
#include <stdint.h>

#ifndef CT_EPS
#define CT_EPS 1e-6f
#endif
#define CT_LN2 0.69314718055994530942f
#define CT_SQRT2 1.414213562373095f
#define CT_HALF_SQRT2 (0.5f * CT_SQRT2)

#define CT_PI 3.1415926535897932384626433832795029f
#define CT_TAU (2.0f * CT_PI)
#define CT_HALF_PI (0.5f * CT_PI)
#define CT_QUARTER_PI (0.25f * CT_PI)
#define CT_INV_TAU (1.0f / CT_TAU)
#define CT_INV_PI (1.0f / CT_PI)
#define CT_INV_HALF_PI (1.0f / CT_HALF_PI)
#define CT_DEGREES (180.0f / CT_PI)
#define CT_RADIANS (CT_PI / 180.0f)

#define CT_INT_BITS1 ((int)(8 * sizeof(int) - 1))

#define CT_MIN(a, b) ((a) < (b) ? (a) : (b))
#define CT_MAX(a, b) ((a) > (b) ? (a) : (b))
#define CT_CLAMP(a, b, c) CT_MIN(CT_MAX(a, b), c)
#define CT_IN_RANGE(x, a, b) ((x) >= (a) && (x) <= (b))
#define CT_SIGNUM(x, eps) (int)((x) < 0 ? -1 : ((x) > 0) ? 1 : 0)
#define CT_COMPARE(a, b) (int)((a) < (b) ? -1 : ((a) > (b) ? 1 : 0))
#define CT_SWAP(type, a, b) \
  do {                      \
    type tmp;               \
    tmp = (a);              \
    (a) = (b);              \
    (b) = tmp;              \
  } while (0)

// x must be 0..1
// bias: 1 = unbiased, <1 = bias towards high, >1 = bias towards low
ct_inline float ct_biasf(const float x,
                         const float low,
                         const float high,
                         const float bias) {
  return low + (high - low) * powf(x, bias);
}

ct_inline size_t ct_clz8(const uint8_t x) {
#if defined(__GCC__) || defined(__clang__) || defined(__EMSCRIPTEN__)
  return x == 0 ? 8 : __builtin_clz(x) - 24;
#else
  return 0;  // FIXME
#endif
}

ct_inline int ct_deltaeqi(const int a, const int b, const int eps) {
  const int t = a - b;
  return (((t - eps) & (-t - eps)) < 0);
}

ct_inline int ct_deltaeqf(const float a, const float b, const float eps) {
  return fabs(a - b) <= eps;
}

ct_inline float ct_maddf(const float a, const float b, const float c) {
  return a * b + c;
}

ct_inline float ct_mixf(const float a, const float b, const float t) {
  return a + (b - a) * t;
}

ct_inline int ct_signumf(const float x, const float eps) {
  if (ct_deltaeqf(x, 0.f, eps)) {
    return 0;
  }
  return (x < 0.f ? -1 : 1);
}

ct_inline float ct_stepf(const float x,
                         const float edge,
                         const float y1,
                         const float y2) {
  return (x < edge ? y1 : y2);
}

ct_inline float ct_wrapf(const float x, const float domain) {
  return ((x < 0.f) ? (domain + x) : (x >= domain ? (x - domain) : x));
}

ct_inline uint32_t ct_rotl32(const uint32_t x, uint8_t r) {
#if __ARM_ARCH == 7
  uint32_t res;
  __asm("ror %0, %1, %2" : "=r"(res) : "r"(x), "I"(32 - r));
  return res;
#else
  return (x << r) | (x >> (32 - r));
#endif
}

ct_inline uint64_t ct_rotl64(const uint64_t x, const uint8_t r) {
  return (x << r) | (x >> (64 - r));
}

ct_inline int ct_is_pow2(size_t x) {
  return (x && ((x & (x - 1)) == 0));
}

ct_inline size_t ct_ceil_pow2(size_t x) {
  if (x) {
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
#ifdef __LP64__
    x |= x >> 32;
#endif
    return x + 1;
  }
  return 1;
}

ct_inline size_t ct_floor_pow2(size_t x) {
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
#ifdef __LP64__
  x |= x >> 32;
#endif
  return x - (x >> 1);
}

ct_inline size_t ct_ceil_multiple_pow2(const size_t x, const size_t y) {
  return (x + (y - 1)) & ~(y - 1);
}

ct_inline int ct_maxi(const int a, const int b) {
  const int t = a - b;
  return a - ((t >> CT_INT_BITS1) & t);
}

ct_inline float ct_maxf(const float a, const float b) {
  return CT_MAX(a, b);
}

ct_inline int ct_mini(const int a, const int b) {
  const int t = b - a;
  return a + ((t >> CT_INT_BITS1) & t);
}

ct_inline float ct_minf(const float a, const float b) {
  return CT_MIN(a, b);
}

ct_inline int ct_clampi(const int x, const int a, const int b) {
  return CT_CLAMP(x, a, b);
}

ct_inline float ct_clampf(const float x, const float a, const float b) {
  return CT_CLAMP(x, a, b);
}

#ifdef __ARM_FEATURE_SAT
ct_inline int16_t ct_clamp16(const int32_t x) {
  uint32_t res;
  __asm("ssat %0, #16, %1" : "=r"(res) : "r"(x));
  return res;
}

ct_inline int32_t ct_clamp24(const int32_t x) {
  uint32_t res;
  __asm("ssat %0, #24, %1" : "=r"(res) : "r"(x));
  return res;
}
#else
ct_inline int16_t ct_clamp16(const int32_t x) {
  return (int16_t)((x < -0x7fff) ? 0x8000U : (x > 0x7fff ? 0x7fff : x));
}
ct_inline int32_t ct_clamp24(const int32_t x) {
  return (int32_t)((x < -0x7fffff) ? 0x800000U : (x > 0x7fffff ? 0x7fffff : x));
}
#endif

ct_inline float ct_mapf(const float x,
                        const float a,
                        const float b,
                        const float c,
                        const float d) {
  return ct_mixf(c, d, (x - a) / (b - a));
}

ct_inline float ct_mapf_clamped(const float x,
                                const float a,
                                const float b,
                                const float c,
                                const float d) {
  return ct_mixf(c, d, ct_clampf((x - a) / (b - a), 0.f, 1.f));
}

// Approximates cos(pi*x) for x in [-1,1]
ct_inline float ct_norm_cos(const float x) {
  const float x2 = x * x;
  return 1.0f + x2 * (-4.0f + 2.0f * x2);
}

ct_inline float ct_fast_cos_impl(const float x) {
  const float x2 = x * x;
  return 0.99940307f + x2 * (-0.49558072f + 0.03679168f * x2);
}

ct_inline float ct_fast_cos(float x) {
  x = fmodf(x, CT_TAU);
  if (x < 0) {
    x = -x;
  }
  switch ((size_t)(x * CT_INV_HALF_PI)) {
    case 0:
      return ct_fast_cos_impl(x);
    case 1:
      return -ct_fast_cos_impl(CT_PI - x);
    case 2:
      return -ct_fast_cos_impl(x - CT_PI);
    default:
      return ct_fast_cos_impl(CT_TAU - x);
  }
}

ct_inline float ct_fast_sin(float x) {
  return ct_fast_cos(CT_HALF_PI - x);
}

CT_END_DECLS
