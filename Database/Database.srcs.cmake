
set(SOURCES "")

set(SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Source/Database.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Database.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/DatabaseElement.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Indexes/Indexes.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Indexes/IndexingOptions.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Indexes/IndexFactory.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Indexes/IndexFactory.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Indexes/IndexCounting.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Indexes/IndexCounting.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Indexes/KmerUtils.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Indexes/NonCopyable.hpp
        )

source_group("Sources" FILES ${SOURCES})
list(APPEND SOURCES ${Sources})
