cmake_minimum_required (VERSION 3.10)
ADD_COMPILE_OPTIONS(-Wall)
project (model-viewer)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")
find_package(SDL2 REQUIRED)
INCLUDE(CTest)
add_subdirectory(src)
