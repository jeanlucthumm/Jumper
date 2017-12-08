#ifndef GLINCLUDE_H
#define GLINCLUDE_H

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/OpenGL.h>
#else
#include <GL/glew.h>
#endif

#endif
