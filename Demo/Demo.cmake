if (NOT TARGET Demo)

    include(${CMAKE_CURRENT_LIST_DIR}/Demo.srcs.cmake)

    add_executable(Tachyon ${SOURCES})
    target_link_libraries(Tachyon PRIVATE TachyonLib)

endif()
