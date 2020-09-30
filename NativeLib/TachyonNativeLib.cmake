if (NOT TARGET TachyonNativeLib)

    find_package(JNI REQUIRED)
    include_directories(${JNI_INCLUDE_DIRS})

    include(${CMAKE_CURRENT_LIST_DIR}/TachyonNativeLib.srcs.cmake)

    add_library(TachyonNativeLib SHARED ${SOURCES})
    target_link_libraries(TachyonNativeLib TachyonLib)
endif()
