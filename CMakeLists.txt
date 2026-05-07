cmake_minimum_required(VERSION 3.20)
project(VexSim CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── SDL2: prefer system package (set SDL2_DIR or USE_SYSTEM_SDL2=ON),
#    fall back to FetchContent build from source ──────────────────────
option(USE_SYSTEM_SDL2 "Use system-installed SDL2 instead of building from source" OFF)

if(DEFINED SDL2_DIR OR USE_SYSTEM_SDL2)
    find_package(SDL2 REQUIRED)
    message(STATUS "Using system SDL2: ${SDL2_DIR}")
    set(SDL2_TARGET SDL2::SDL2)          # shared DLL on MSYS2
else()
    include(FetchContent)
    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        release-2.30.3
        GIT_SHALLOW    TRUE
    )
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON  CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(SDL2)
    set(SDL2_TARGET SDL2::SDL2-static)   # static lib from FetchContent
endif()

find_package(Threads REQUIRED)

add_subdirectory(stubs)
add_subdirectory(lemlib)   # no-op if LEMLIB_DIR not set
add_subdirectory(robot)
add_subdirectory(sim)
