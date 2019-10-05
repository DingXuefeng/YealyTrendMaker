cmake_minimum_required (VERSION 2.6)
project (main)
include_directories(include)
add_executable(main main.cxx)
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
target_link_libraries(main ${ROOT_LIBRARIES})
add_subdirectory(src)
target_link_libraries (main TrendLib)





