//         __          .__                       .___
//   _____/  |_        |  |__   ____ _____     __| _/
// _/ ___\   __\ ______|  |  \_/ __ \\__  \   / __ |
// \  \___|  |  /_____/|   Y  \  ___/ / __ \_/ /_/ |
//  \___  >__|         |___|  /\___  >____  /\____ |
//      \/                  \/     \/     \/      \/
//

#pragma once

#include "features.h"
#include "log.h"

CT_BEGIN_DECLS

#include <time.h>

#define CT_TEST_DECLS       \
  extern size_t __testsRun; \
  extern size_t __testAsserts;

#define CT_IS(test, message, ...)     \
  __testAsserts++;                    \
  if (!(test)) {                      \
    CT_ERROR(message, ##__VA_ARGS__); \
    return 1;                         \
  }

#define CT_RUN_TEST(test)                                                  \
  CT_INFO("----- %s -----", "" #test);                                     \
  do {                                                                     \
    size_t t        = __testAsserts;                                       \
    clock_t begin   = clock();                                             \
    int res         = test();                                              \
    double measured = (double)(clock() - begin) / CLOCKS_PER_SEC * 1000.0; \
    CT_INFO("%zu assertions (%1.3fms)", __testAsserts - t, measured);      \
    __testsRun++;                                                          \
    if (res) {                                                             \
      return res;                                                          \
    }                                                                      \
  } while (0);

#define CT_RUN_TESTS(name)                                                    \
  int main() {                                                                \
    CT_INFO("----- RUN TESTS: %s -----", "" #name);                           \
    int result = name();                                                      \
    CT_INFO("Tests run: %zu, total asserts: %zu", __testsRun, __testAsserts); \
    if (!result) {                                                            \
      CT_SUCCESS("ALL TESTS PASSED");                                         \
    }                                                                         \
    return result;                                                            \
  }

CT_END_DECLS
