set(SOURCES "")

set(SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Source/jnipp.h
        ${CMAKE_CURRENT_LIST_DIR}/Source/jnimacros.h
        ${CMAKE_CURRENT_LIST_DIR}/Source/TachyonJNI.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/TachyonJNI.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/TachyonResultJNI.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/TachyonResultJNI.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/TachyonResultEntryJNI.cpp
        )

source_group("Sources" FILES ${SOURCES})
list(APPEND SOURCES ${Sources})
