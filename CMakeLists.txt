cmake_minimum_required(VERSION 3.5.1)

project(Tachyon)

add_compile_options(
        "-O3"
        "-std=gnu++1z"
        "-DNDEBUG"
        "-Wall"
        "-Wextra"
        "-Werror"
        "-Wno-error=deprecated-declarations"
        "$<$<CXX_COMPILER_ID:Clang>:-Wshorten-64-to-32>"
        "-fno-exceptions"
        "-fno-rtti"
        "-fno-stack-protector"
        "-fPIC"
        "-march=native"
        "-fvisibility=hidden"
        "-fvisibility-inlines-hidden"

        "$<$<CONFIG:DEBUG>:-g>"
        "$<$<CONFIG:MEMCHECK>:-g>"
        "$<$<CONFIG:MEMCHECK>:-fsanitize=address;-fsanitize=undefined>"


)

link_libraries(
        "pthread"
        "$<$<CONFIG:MEMCHECK>:-fsanitize=address>"
        "$<$<CONFIG:MEMCHECK>:-fsanitize=undefined>")

include(${CMAKE_CURRENT_LIST_DIR}/Database/Database.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Utils/Utils.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/Aligner/Aligner.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/TachyonLib/TachyonLib.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/Demo/Demo.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/NativeLib/TachyonNativeLib.cmake)
