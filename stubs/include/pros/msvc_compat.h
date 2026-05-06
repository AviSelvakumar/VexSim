#pragma once
// MSVC compatibility shims for GCC/Clang extensions used in PROS headers

#ifdef _MSC_VER

// Suppress GCC __attribute__ syntax (packed, deprecated, etc.)
// This means packed structs won't be packed, but that's fine for simulation.
#ifndef __attribute__
#  define __attribute__(x)
#endif

// Suppress MSVC warnings commonly triggered by PROS headers
#pragma warning(disable: 4068)  // unknown pragma
#pragma warning(disable: 4200)  // zero-sized array in struct/union

#endif  // _MSC_VER
