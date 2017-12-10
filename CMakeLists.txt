cmake_minimum_required(VERSION 3.9)
project(Jumper)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(PkgConfig)
pkg_search_module(GLFW REQUIRED glfw3)

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

set(STB_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_image.h
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/optstb.cpp)

add_executable(jumper ${SOURCE_FILES} ${STB_LIBRARIES})

target_include_directories(jumper PUBLIC ${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR})
target_link_libraries(jumper ${OPENGL_gl_LIBRARY} ${GLUT_LIBRARIES}
        ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})