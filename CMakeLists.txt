set(CMAKE_CXX_STANDARD 11)

cmake_minimum_required(VERSION 3.0.0)
project(model VERSION 0.1.0)

add_executable(model src/main.cpp)

target_link_libraries(model soil2 glfw3 dl X11 pthread GLEW GLU GL)