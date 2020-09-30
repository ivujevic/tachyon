
set(SOURCES "")

set(SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Source/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Writers/Writer.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Writers/Writer.cpp
)

source_group("Sources" FILES ${SOURCES})
list(APPEND SOURCES ${Sources})
