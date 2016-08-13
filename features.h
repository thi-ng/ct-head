//         __          .__                       .___
//   _____/  |_        |  |__   ____ _____     __| _/
// _/ ___\   __\ ______|  |  \_/ __ \\__  \   / __ |
// \  \___|  |  /_____/|   Y  \  ___/ / __ \_/ /_/ |
//  \___  >__|         |___|  /\___  >____  /\____ |
//      \/                  \/     \/     \/      \/
//

#pragma once

#if defined(CT_FEATURE_CHECKS) && !defined(CT_FEATURE_LOG)
#define CT_FEATURE_LOG 1
#endif

#if defined(CT_FEATURE_SSE) && !defined(__SSE__)
#undef CT_FEATURE_SSE 1
#endif

#if defined(CT_FEATURE_SSE) && defined(__SSE2__)
#define CT_FEATURE_SSE2 1
#endif

#if defined(CT_FEATURE_SSE) && defined(__SSE3__)
#define CT_FEATURE_SSE3 1
#endif

#if defined(CT_FEATURE_SSE) && defined(__SSE4_1__)
#define CT_FEATURE_SSE4 1
#endif

#if defined(CT_FEATURE_SSE) && defined(__AVX__)
#define CT_FEATURE_AVX 1
#endif

#if defined(CT_FEATURE_SSE) && defined(__AVX2__)
#define CT_FEATURE_AVX2 1
#endif

#ifdef CT_FEATURE_ANSI
#define CT_ANSI_RED "\x1b[31;1m"
#define CT_ANSI_GREEN "\x1b[32;1m"
#define CT_ANSI_YELLOW "\x1b[33;1m"
#define CT_ANSI_BLUE "\x1b[34;1m"
#define CT_ANSI_MAGENTA "\x1b[35;1m"
#define CT_ANSI_CYAN "\x1b[36;1m"
#define CT_ANSI_WHITE "\x1b[37;1m"
#define CT_ANSI_RESET "\x1b[0m"
#else
#define CT_ANSI_RED
#define CT_ANSI_GREEN
#define CT_ANSI_YELLOW
#define CT_ANSI_BLUE
#define CT_ANSI_MAGENTA
#define CT_ANSI_CYAN
#define CT_ANSI_WHITE
#define CT_ANSI_RESET
#endif

#if defined(__EMSCRIPTEN__) && !defined(CT_NO_EXPORT)
#define ct_export __attribute__((used))
#else
#define ct_export
#endif

#define ct_inline static inline

#ifdef __cplusplus
#define CT_BEGIN_DECLS extern "C" {
#define CT_END_DECLS }
#else
#define CT_BEGIN_DECLS
#define CT_END_DECLS
#endif
