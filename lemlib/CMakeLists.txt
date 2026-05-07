# ─────────────────────────────────────────────────────────────────────────────
# LemLib + LemLog CMake target for the VEX Simulator
#
# Usage: set LEMLIB_DIR to the root of your LemLib source checkout, e.g.:
#   cmake -DLEMLIB_DIR="C:/Users/avise/LemLib" --preset msvc-x64
#
# Or add to CMakePresets.json:
#   "cacheVariables": { "LEMLIB_DIR": "C:/Users/avise/LemLib" }
# ─────────────────────────────────────────────────────────────────────────────

if(NOT DEFINED LEMLIB_DIR OR LEMLIB_DIR STREQUAL "")
    message(STATUS "LEMLIB_DIR not set — LemLib will not be compiled.")
    message(STATUS "  To enable: cmake -DLEMLIB_DIR=<path-to-LemLib-repo> ...")
    return()
endif()

if(NOT EXISTS "${LEMLIB_DIR}/src/lemlib")
    message(FATAL_ERROR "LEMLIB_DIR='${LEMLIB_DIR}' does not contain src/lemlib. "
                        "Point it at the root of the LemLib source repository.")
endif()

message(STATUS "Building LemLib from: ${LEMLIB_DIR}")

# Collect all LemLib source files
file(GLOB_RECURSE LEMLIB_SOURCES
    "${LEMLIB_DIR}/src/lemlib/*.cpp"
)

# LemLog stub is only needed for LemLib versions that use it (>= 0.6.x)
set(EXTRA_SOURCES "")
if(EXISTS "${LEMLIB_DIR}/include/LemLog")
    list(APPEND EXTRA_SOURCES lemlog_stub.cpp)
    message(STATUS "LemLog detected — including lemlog_stub.cpp")
endif()

add_library(lemlib STATIC
    ${LEMLIB_SOURCES}
    ${EXTRA_SOURCES}
)

target_include_directories(lemlib
    PUBLIC
        "${LEMLIB_DIR}/include"          # lemlib/, hardware/, units/, LemLog/, hot-cold-asset/
)
# Put stubs BEFORE LemLib's bundled pros/ headers so our type definitions win
target_include_directories(lemlib BEFORE PRIVATE
    ${CMAKE_SOURCE_DIR}/stubs/include    # pros/ headers (must precede LEMLIB_DIR/include)
    "${LEMLIB_DIR}/include/lemlib"       # bare includes (e.g. #include "pid.hpp")
)

target_link_libraries(lemlib
    PUBLIC  pros_stubs
    PUBLIC  Threads::Threads
)

# LemLib uses C++23 lambda syntax: [&] -> Type {} without ()
target_compile_features(lemlib PUBLIC cxx_std_23)

set(LEMLIB_COMPAT_H "${CMAKE_CURRENT_SOURCE_DIR}/lemlib_compat.h")

if(MSVC)
    target_compile_options(lemlib PRIVATE
        /W2 /wd4244 /wd4267 /wd4305 /wd4996
        "/FI${LEMLIB_COMPAT_H}"
        /std:c++latest
    )
else()
    # GCC/Clang: -include force-includes the compat header; -std=gnu++23 for lambda syntax
    target_compile_options(lemlib PRIVATE
        -w
        "-include${LEMLIB_COMPAT_H}"
        -std=gnu++23
    )
endif()
