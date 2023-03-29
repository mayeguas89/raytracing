cmake_minimum_required(VERSION 3.0.0)
project(RTIOW VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(glm)
add_subdirectory(fmt)

add_executable(RTIOW src/main.cpp)
target_include_directories(RTIOW PUBLIC src)
target_link_libraries(RTIOW PUBLIC glm::glm fmt::fmt)