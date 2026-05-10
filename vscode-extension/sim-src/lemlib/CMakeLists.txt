# ─────────────────────────────────────────────────────────────────────────────
# LemLib CMake target for the VEX Simulator (supports 0.5.x)
#
# Set LEMLIB_DIR to the root of the LemLib 0.5.x source checkout, e.g.:
#   cmake -DLEMLIB_DIR="C:/Users/avise/LemLib-0.5.6" --preset msys2-ucrt64-lemlib
# ─────────────────────────────────────────────────────────────────────────────

if(NOT DEFINED LEMLIB_DIR OR LEMLIB_DIR STREQUAL "")
    message(STATUS "LEMLIB_DIR not set — LemLib will not be compiled.")
    return()
endif()

if(NOT EXISTS "${LEMLIB_DIR}/src/lemlib")
    message(FATAL_ERROR "LEMLIB_DIR='${LEMLIB_DIR}' does not contain src/lemlib. "
                        "Point it at the root of the LemLib 0.5.x source repository.")
endif()

message(STATUS "Building LemLib from: ${LEMLIB_DIR}")

file(GLOB_RECURSE LEMLIB_SOURCES "${LEMLIB_DIR}/src/lemlib/*.cpp")

add_library(lemlib STATIC ${LEMLIB_SOURCES})

target_include_directories(lemlib
    PUBLIC
        "${LEMLIB_DIR}/include"   # lemlib/, pros/, fmt/
)

# stubs/include must precede LemLib's bundled pros/ headers so our type definitions win.
# Also add include/lemlib as a bare search path (some .cpp files do #include "pid.hpp" etc.)
target_include_directories(lemlib BEFORE PRIVATE
    ${CMAKE_SOURCE_DIR}/stubs/include
    "${LEMLIB_DIR}/include/lemlib"
)

target_link_libraries(lemlib
    PUBLIC pros_stubs
    PUBLIC Threads::Threads
)

target_compile_features(lemlib PUBLIC cxx_std_17)

set(LEMLIB_COMPAT_H "${CMAKE_CURRENT_SOURCE_DIR}/lemlib_compat.h")

if(MSVC)
    target_compile_options(lemlib PRIVATE
        /W2 /wd4244 /wd4267 /wd4305 /wd4996
        "/FI${LEMLIB_COMPAT_H}"
    )
else()
    target_compile_options(lemlib PRIVATE
        -w
        "-include${LEMLIB_COMPAT_H}"
    )
endif()
