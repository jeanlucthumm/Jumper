//
// Created by Jean-Luc Thumm on 1/2/18.
//

#ifndef JUMPER_GLUTIL_HPP
#define JUMPER_GLUTIL_HPP

#include "GLinclude.h"
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <vector>

namespace glutil {

GLuint makeStdVAO(const std::vector<glm::vec3> &vertices);

GLuint makeStdVAO(const std::vector<glm::vec3> &vertices,
                  const std::vector<glm::vec3> &normals);

GLuint makeStdVAO(const std::vector<glm::vec3> &vertices,
                  const std::vector<glm::vec3> &normals,
                  const std::vector<glm::vec2> &uvs);
}

#endif //JUMPER_GLUTIL_HPP
