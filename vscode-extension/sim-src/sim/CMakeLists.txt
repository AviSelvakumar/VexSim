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
    PUBLIC  ${SDL2_TARGET}
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
    # Use lld (LLVM linker) — avoids GNU ld silent crash on duplicate symbols.
    # --allow-multiple-definition: LemLib headers define some PROS class bodies
    # inline, which duplicate our stub definitions. lld rejects; this silences it
    # and uses the first definition encountered (robot_code, i.e. LemLib's).
    target_link_options(vex_sim PRIVATE
        -fuse-ld=lld
        -Wl,--allow-multiple-definition
    )
endif()

# ── Copy SDL2.dll next to the exe so it can find it at runtime ──────────────
# This is needed when using the system SDL2 shared library (MSYS2 package).
if(DEFINED SDL2_DIR)
    find_file(SDL2_DLL_PATH
        NAMES SDL2.dll
        HINTS "${SDL2_DIR}/../../../bin"      # C:/msys64/ucrt64/bin
        NO_DEFAULT_PATH
    )
    if(SDL2_DLL_PATH)
        set(_SDL2_BIN_DIR "${SDL2_DIR}/../../../bin")
        add_custom_command(TARGET vex_sim POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${SDL2_DLL_PATH}"
                "$<TARGET_FILE_DIR:vex_sim>/SDL2.dll"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${_SDL2_BIN_DIR}/libgcc_s_seh-1.dll"
                "$<TARGET_FILE_DIR:vex_sim>/libgcc_s_seh-1.dll"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${_SDL2_BIN_DIR}/libstdc++-6.dll"
                "$<TARGET_FILE_DIR:vex_sim>/libstdc++-6.dll"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
                "${_SDL2_BIN_DIR}/libwinpthread-1.dll"
                "$<TARGET_FILE_DIR:vex_sim>/libwinpthread-1.dll"
            COMMENT "Copying SDL2.dll and runtime DLLs to build output directory"
        )
    endif()
endif()
