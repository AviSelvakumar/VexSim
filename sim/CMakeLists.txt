add_library(sim_engine STATIC
    src/SimState.cpp
    src/Physics.cpp
    src/Renderer.cpp
    src/InputHandler.cpp
    src/Scheduler.cpp
)

target_include_directories(sim_engine
    PUBLIC  include
    PRIVATE ${CMAKE_SOURCE_DIR}/stubs/include
)

target_link_libraries(sim_engine
    PUBLIC  SDL2::SDL2-static
    PUBLIC  Threads::Threads
)

add_executable(vex_sim
    src/main.cpp
)

target_link_libraries(vex_sim
    PRIVATE SDL2::SDL2main
    PRIVATE sim_engine
    PRIVATE robot_code
    PRIVATE pros_stubs
    PRIVATE Threads::Threads
)

if(MSVC)
    set(COMPAT_H "${CMAKE_SOURCE_DIR}/stubs/include/pros/msvc_compat.h")
    target_compile_options(sim_engine PRIVATE /W3 /wd4244 "/FI${COMPAT_H}")
    target_compile_options(vex_sim PRIVATE /W3 "/FI${COMPAT_H}")
else()
    # GCC/Clang: need pthread and math
    target_link_libraries(vex_sim
        PRIVATE -lpthread
        PRIVATE -lm
    )
endif()
