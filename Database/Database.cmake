if (NOT TARGET Database)

    include(${CMAKE_CURRENT_LIST_DIR}/Database.srcs.cmake)
    add_library(Database ${SOURCES})

#    target_sources(Database PRIVATE ${SOURCES})

    target_link_libraries(Database PRIVATE Utils)
endif()
