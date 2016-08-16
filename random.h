//         __          .__                       .___
//   _____/  |_        |  |__   ____ _____     __| _/
// _/ ___\   __\ ______|  |  \_/ __ \\__  \   / __ |
// \  \___|  |  /_____/|   Y  \  ___/ / __ \_/ /_/ |
//  \___  >__|         |___|  /\___  >____  /\____ |
//      \/                  \/     \/     \/      \/
//

#pragma once

#include "features.h"
#include "math.h"

CT_BEGIN_DECLS

#include <stdint.h>
#include <stdlib.h>

#define CT_INV_RAND_MAX (1.0f / RAND_MAX)
#define CT_INV_RAND_MAX2 (2.0f / RAND_MAX)
#define CT_INV_UINT32_MAX (1.0f / UINT32_MAX)
#define CT_INV_UINT32_MAX2 (2.0f / UINT32_MAX)

typedef struct {
  uint32_t seed;
  uint32_t iter;
} CT_Smush;

typedef struct {
  uint32_t x;
  uint32_t y;
  uint32_t z;
  uint32_t w;
} CT_XorShift;

// ARM freestanding has no libc rand
#ifdef __STDC_HOSTED__
ct_inline float ct_rand_norm() {
  return rand() * CT_INV_RAND_MAX2 - 1.0f;
}

ct_inline float ct_rand_normpos() {
  return rand() * CT_INV_RAND_MAX;
}

ct_inline float ct_rand_minmax(const float min, const float max) {
  return ct_mixf(min, max, ct_rand_normpos());
}
#endif

// https://gist.github.com/voidqk/d112165a26b45244a65298933c0349a4

ct_inline void ct_smush_init(CT_Smush *rnd, const uint32_t s) {
  rnd->seed = s;
  rnd->iter = 0;
}

ct_inline uint32_t ct_smush(CT_Smush *rnd) {
  const uint32_t m = 0x5bd1e995;
  const uint32_t k = rnd->iter++ * m;
  rnd->seed        = (k ^ (k >> 24) ^ (rnd->seed * m)) * m;
  return rnd->seed ^ (rnd->seed >> 13);
}

ct_inline float ct_smush_norm(CT_Smush *rnd) {
  return (float)ct_smush(rnd) * CT_INV_UINT32_MAX2 - 1.f;
}

ct_inline float ct_smush_normpos(CT_Smush *rnd) {
  return (float)ct_smush(rnd) * CT_INV_UINT32_MAX;
}

ct_inline float ct_smush_minmax(CT_Smush *rnd,
                                const float min,
                                const float max) {
  return ct_mixf(min, max, ct_smush_normpos(rnd));
}

// xorshift128 - https://en.wikipedia.org/wiki/Xorshift

ct_inline void ct_xors_init(CT_XorShift *rnd) {
  rnd->x = 0xdecafbad;
  rnd->y = 0x2fa9d05b;
  rnd->z = 0x041f67e3;
  rnd->w = 0x5c83ec1a;
}

ct_inline uint32_t ct_xors(CT_XorShift *rnd) {
  uint32_t t = rnd->x;
  t ^= t << 11;
  t ^= t >> 8;
  rnd->x = rnd->y;
  rnd->y = rnd->z;
  rnd->z = rnd->w;
  rnd->w ^= rnd->w >> 19;
  return rnd->w ^= t;
}

ct_inline float ct_xors_norm(CT_XorShift *rnd) {
  return (float)ct_xors(rnd) * CT_INV_UINT32_MAX2 - 1.f;
}

ct_inline float ct_xors_normpos(CT_XorShift *rnd) {
  return (float)ct_xors(rnd) * CT_INV_UINT32_MAX;
}

ct_inline float ct_xors_minmax(CT_XorShift *rnd,
                               const float min,
                               const float max) {
  return ct_mixf(min, max, ct_xors_normpos(rnd));
}

CT_END_DECLS
