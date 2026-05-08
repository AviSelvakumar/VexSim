# If ROBOT_SRC_DIR is set (e.g. by the VSCode extension), compile directly from
# the PROS project's src/ directory. Otherwise fall back to the local main.cpp.
if(DEFINED ROBOT_SRC_DIR AND NOT ROBOT_SRC_DIR STREQUAL "")
    file(GLOB_RECURSE ROBOT_SOURCES "${ROBOT_SRC_DIR}/*.cpp")
    message(STATUS "robot_code: using PROS project sources from ${ROBOT_SRC_DIR}")
else()
    file(GLOB_RECURSE ROBOT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
endif()

add_library(robot_code STATIC ${ROBOT_SOURCES})

target_include_directories(robot_code PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(DEFINED ROBOT_INCLUDE_DIR AND NOT ROBOT_INCLUDE_DIR STREQUAL "")
    target_include_directories(robot_code PUBLIC "${ROBOT_INCLUDE_DIR}")
endif()

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
