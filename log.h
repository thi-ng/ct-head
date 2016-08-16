//         __          .__                       .___
//   _____/  |_        |  |__   ____ _____     __| _/
// _/ ___\   __\ ______|  |  \_/ __ \\__  \   / __ |
// \  \___|  |  /_____/|   Y  \  ___/ / __ \_/ /_/ |
//  \___  >__|         |___|  /\___  >____  /\____ |
//      \/                  \/     \/     \/      \/
//

#pragma once

#include "features.h"

CT_BEGIN_DECLS

#ifdef CT_FEATURE_LOG

#include <stdio.h>

#ifndef CT_LOG_PRELUDE
#define CT_LOG_PRELUDE fprintf(stderr,
#endif

#ifdef NDEBUG
#define CT_DEBUG(M, ...)
#else
#define CT_DEBUG(M, ...) \
  CT_LOG_PRELUDE "[  DEBUG] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif  // NDEBUG

#define CT_ERROR(M, ...)                                         \
  CT_LOG_PRELUDE                                                 \
          CT_ANSI_RED "[  ERROR] (%s:%d) " M CT_ANSI_RESET "\n", \
	  __FILE__, __LINE__, ##__VA_ARGS__)

#define CT_WARN(M, ...)                                           \
  CT_LOG_PRELUDE "[   WARN] (%s:%d) " M "\n", __FILE__, __LINE__, \
	  ##__VA_ARGS__)

#define CT_INFO(M, ...) \
  CT_LOG_PRELUDE "[   INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define CT_SUCCESS(M, ...)                                                \
  CT_LOG_PRELUDE CT_ANSI_GREEN "[SUCCESS] (%s:%d) " M CT_ANSI_RESET "\n", \
	  __FILE__, __LINE__, ##__VA_ARGS__)

#define CT_TIMED(...)                                                        \
  {                                                                          \
    clock_t t0 = clock();                                                    \
    __VA_ARGS__;                                                             \
    CT_INFO("time: %1.3fms", (double)(clock() - t0) / CLOCKS_PER_SEC * 1e3); \
  }

#else  // CT_FEATURE_LOG

#define CT_DEBUG(M, ...)
#define CT_ERROR(M, ...)
#define CT_WARN(M, ...)
#define CT_INFO(M, ...)
#define CT_SUCCESS(M, ...)

#define CT_TIMED(...) __VA_ARGS__

#endif  // CT_FEATURE_LOG

////////////////////////////////////////////////////////////

#ifndef CT_CHECK_FAIL_ACTION
#define CT_CHECK_FAIL_ACTION goto fail
#endif

#ifdef CT_FEATURE_CHECKS

#define CT_CHECK(A, M, ...)     \
  if (!(A)) {                   \
    CT_ERROR(M, ##__VA_ARGS__); \
    CT_CHECK_FAIL_ACTION;       \
  }

#define CT_CHECK_DEBUG(A, M, ...) \
  if (!(A)) {                     \
    CT_DEBUG(M, ##__VA_ARGS__);   \
    CT_CHECK_FAIL_ACTION;         \
  }

#else  // CT_FEATURE_CHECKS

#define CT_CHECK(A, M, ...)
#define CT_CHECK_DEBUG(A, M, ...)

#endif  // CT_FEATURE_CHECKS

#define CT_SENTINEL(M, ...)     \
  {                             \
    CT_ERROR(M, ##__VA_ARGS__); \
    CT_CHECK_FAIL_ACTION;       \
  }

////////////////////////////////////////////////////////////

#if CT_FEATURE_CHECK_MEM

#ifndef CT_CHECK_MEM_FAIL_ACTION
#define CT_CHECK_MEM_FAIL_ACTION exit(1)
#endif

#define CT_CHECK_MEM(A)         \
  if (!(A)) {                   \
    CT_ERROR("Out of memory!"); \
    CT_CHECK_MEM_FAIL_ACTION;   \
  }

#else  // CT_FEATURE_CHECK_MEM

#define CT_CHECK_MEM(A) A

#endif  // CT_FEATURE_CHECK_MEM

CT_END_DECLS
