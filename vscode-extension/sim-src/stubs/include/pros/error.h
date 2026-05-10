#pragma once
#include <cerrno>
#include <cstdint>
#include <cmath>  // for INFINITY

#define PROS_ERR      (INT32_MAX)
#define PROS_ERR_F    (INFINITY)
#define PROS_ERR_BYTE (UINT8_MAX)

// MSVC does not support GCC's __attribute__ syntax.
// Map it to nothing so real PROS headers compile under MSVC.
#ifdef _MSC_VER
#  ifndef __attribute__
#    define __attribute__(x)
#  endif
#  pragma warning(disable: 4068)  // unknown pragma
#endif
