add_library(robot_code STATIC
    main.cpp
)

target_include_directories(robot_code
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(robot_code
    PUBLIC pros_stubs
)

if(MSVC)
    set(COMPAT_H "${CMAKE_SOURCE_DIR}/stubs/include/pros/msvc_compat.h")
    target_compile_options(robot_code PRIVATE /W3 /wd4244 "/FI${COMPAT_H}")
endif()
