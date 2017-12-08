//
// Created by Jean-Luc Thumm on 10/26/17.
//

#ifndef PA2_MATERIAL_H
#define PA2_MATERIAL_H


#include <glm/vec3.hpp>

struct Material {
public:
    glm::vec3 ka;
    glm::vec3 kd;
    glm::vec3 ks;
    float shiny;
};


#endif //PA2_MATERIAL_H
