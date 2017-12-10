cmake_minimum_required(VERSION 3.9)
project(Jumper)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Boost COMPONENTS filesystem REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(STB_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_image.h
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/optstb.cpp)

add_executable(jumper ${SOURCE_FILES} ${STB_LIBRARIES})

target_link_libraries(jumper GLUT::GLUT GLEW::GLEW
        ${GLFW_LIBRARIES} OpenGL::GL Boost::boost Boost::filesystem)