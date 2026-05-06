add_library(pros_stubs STATIC
    src/rtos.cpp
    src/motors.cpp
    src/misc.cpp
    src/imu.cpp
    src/rotation.cpp
    src/llemu.cpp
    src/device.cpp
    src/apix.cpp
    src/optical.cpp
    src/gps.cpp
    src/vision.cpp
    src/distance.cpp
    src/serial.cpp
    src/link.cpp
    src/screen.cpp
    src/ext_adi.cpp
)

target_include_directories(pros_stubs
    PUBLIC  include
    PRIVATE ${CMAKE_SOURCE_DIR}/sim/include
)

target_link_libraries(pros_stubs
    PUBLIC Threads::Threads
)

if(MSVC)
    # Force-include the MSVC compatibility shim before every source file.
    # This maps GCC's __attribute__(x) to nothing, fixing real PROS headers.
    set(COMPAT_H "${CMAKE_SOURCE_DIR}/stubs/include/pros/msvc_compat.h")
    target_compile_options(pros_stubs PRIVATE
        /W3 /wd4244 /wd4267 /wd4305
        "/FI${COMPAT_H}"
    )
endif()
