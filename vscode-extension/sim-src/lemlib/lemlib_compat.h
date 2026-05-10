// Compatibility shim force-included before all LemLib source files under MSVC.
// GCC/Clang pull these in transitively and define these extensions; MSVC does not.
#pragma once

#ifdef _MSC_VER

// ── Suppress __attribute__ and GCC pragmas ────────────────────────────────────
#  ifndef __attribute__
#    define __attribute__(x)
#  endif
#pragma warning(disable: 4068)  // unknown pragma
#pragma warning(disable: 4200)  // zero-sized array
#pragma warning(disable: 4244)  // narrowing conversion
#pragma warning(disable: 4267)  // size_t narrowing
#pragma warning(disable: 4305)  // double→float truncation
#pragma warning(disable: 4996)  // deprecated functions

// ── GCC string/math built-ins ─────────────────────────────────────────────────
#  define __builtin_strcmp strcmp
#  define __builtin_unreachable() __assume(false)

// ── M_PI and friends (requires _USE_MATH_DEFINES before <cmath>) ─────────────
#  ifndef _USE_MATH_DEFINES
#    define _USE_MATH_DEFINES
#  endif

#endif // _MSC_VER

// ── Standard headers LemLib uses without explicitly including ─────────────────
#include <cmath>

// ── PROS infinity() helper (not standard C++) ────────────────────────────────
#include <limits>
inline float infinity() { return std::numeric_limits<float>::infinity(); }

// ── isnanf / isinff not available on MinGW — map to the generic versions ──────
#ifndef isnanf
#  define isnanf(x) (std::isnan((float)(x)))
#endif
#ifndef isinff
#  define isinff(x) (std::isinf((float)(x)))
#endif
#include <optional>
#include <variant>
#include <functional>
#include <numbers>

// ── Math constants missing from MSVC even with _USE_MATH_DEFINES ──────────────
#ifndef M_PI
#  define M_PI   3.14159265358979323846
#endif
#ifndef M_PI_2
#  define M_PI_2 1.57079632679489661923
#endif
#ifndef M_PI_4
#  define M_PI_4 0.78539816339744830962
#endif
#ifndef M_TWOPI
#  define M_TWOPI 6.28318530717958647692
#endif
#ifndef M_SQRT2
#  define M_SQRT2 1.41421356237309504880
#endif
