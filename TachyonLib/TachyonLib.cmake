if (NOT TARGET TachyonLib)

    include(${CMAKE_CURRENT_LIST_DIR}/TachyonLib.srcs.cmake)

    add_library(TachyonLib ${SOURCES})
    target_link_libraries(TachyonLib PRIVATE Database Utils)

endif()
