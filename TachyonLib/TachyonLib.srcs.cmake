
set(SOURCES "")

set(SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/Source/Query.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Steps/First/FirstStep.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Steps/First/FirstStep.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Steps/First/FirstStepResult.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Steps/First/FirstStepResultEntry.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Steps/First/FirstStepOptions.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Steps/First/HitsFinder.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Steps/First/HitsFinder.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Steps/Second/SecondStep.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Steps/Second/SecondStep.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/TachyonResultJNI.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/TachyonResultEntry.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Tachyon.hpp
        ${CMAKE_CURRENT_LIST_DIR}/Source/Tachyon.cpp
        )

source_group("Sources" FILES ${SOURCES})
list(APPEND SOURCES ${Sources})
