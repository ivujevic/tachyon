
set(SOURCES "")

set(SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Source/FastaFileUtils.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/FastaFileUtils.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Logger.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Logger.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/StringStream.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/StringStream.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Seg.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Seg.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/LogArray.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/ThreadPool.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Timer.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Timer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/BinaryStream.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/BinaryStream.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Visitor.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/DNATranslator.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/DNATranslator.cpp

        )

source_group("Sources" FILES ${SOURCES})
list(APPEND SOURCES ${Sources})
