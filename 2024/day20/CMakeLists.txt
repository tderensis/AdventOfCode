cmake_path(GET CMAKE_CURRENT_SOURCE_DIR FILENAME Name)
add_executable(${Name} solution.cpp)
set_property(TARGET ${Name} PROPERTY CXX_STANDARD 23)
