add_library(robot_code STATIC
    main.cpp
    # Add any other .cpp files from your PROS project's src/ here, e.g.:
    # autons.cpp
    # chassis.cpp
)

target_include_directories(robot_code
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(robot_code
    PUBLIC pros_stubs
)

if(TARGET lemlib)
    target_link_libraries(robot_code PUBLIC lemlib)
    # stubs/include MUST come before LEMLIB_DIR/include so our pros/ headers
    # shadow LemLib's bundled copies (type names must match stub implementations).
    # BEFORE prepends ahead of all existing includes on the target.
    target_include_directories(robot_code BEFORE PUBLIC
        "${CMAKE_SOURCE_DIR}/stubs/include"
        "${LEMLIB_DIR}/include"
    )
    message(STATUS "robot_code: LemLib linked in")
endif()

if(MSVC)
    set(COMPAT_H "${CMAKE_SOURCE_DIR}/stubs/include/pros/msvc_compat.h")
    target_compile_options(robot_code PRIVATE
        /W3 /wd4244
        "/FI${COMPAT_H}"
        /D__builtin_strcmp=strcmp
    )
endif()
