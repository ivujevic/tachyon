if (NOT TARGET TachyonPy)

    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/pybind11)
    include(${CMAKE_CURRENT_LIST_DIR}/PythonLib.srcs.cmake)

    pybind11_add_module(TachyonPy ${SOURCES})
    target_link_libraries(TachyonPy PRIVATE TachyonLib)
endif()
