if (NOT TARGET Aligner)

    include(${CMAKE_CURRENT_LIST_DIR}/Aligner.srcs.cmake)

    add_library(Aligner ${SOURCES})

    target_link_libraries(Aligner Utils)
#    target_link_libraries(tachyon PRIVATE Database Utils)

endif()
