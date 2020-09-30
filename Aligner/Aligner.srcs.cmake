
set(SOURCES "")

set(SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Source/ScoreMatrix.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Aligner.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Aligner.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/BlosumTables.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/EValue.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/EValue.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Alignment.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Opal/opal.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Opal/opal.h
        )

source_group("Sources" FILES ${SOURCES})
list(APPEND SOURCES ${Sources})
