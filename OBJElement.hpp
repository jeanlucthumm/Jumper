//
// Created by Jean-Luc Thumm on 12/10/17.
//

#ifndef JUMPER_OBJELEMENT_HPP
#define JUMPER_OBJELEMENT_HPP

#include "GLinclude.h"
#include "Material.h"
#include <memory>
#include <string>
#include <vector>
#include <glm/vec3.hpp>

struct OBJElement {
    OBJElement() : VAO{0} {}

    GLuint VAO;
    std::shared_ptr<const Material> material;
    std::string name;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<unsigned int> indices;

    glm::vec3 max;
    glm::vec3 min;
};


#endif //JUMPER_OBJELEMENT_HPP
