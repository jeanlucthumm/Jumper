//
// Created by Jean-Luc Thumm on 11/25/17.
//

#ifndef PA4_UTIL_H
#define PA4_UTIL_H

#include "GLinclude.h"
#include <string>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <vector>

namespace util {

void print_error(GLenum error);

std::string error_string(GLenum error);

void before_error();

void after_error();

glm::vec4 rgba_normalize(float r, float g, float b);

void print(std::string_view name, const std::vector<glm::vec3> &vec);
void print(std::string_view name, const std::vector<glm::vec2> &vec);
}

#endif //PA4_UTIL_H
