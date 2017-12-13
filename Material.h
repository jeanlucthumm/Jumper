//
// Created by Jean-Luc Thumm on 10/26/17.
//

#ifndef PA2_MATERIAL_H
#define PA2_MATERIAL_H


#include "Shader.h"
#include "GLinclude.h"
#include <glm/vec3.hpp>
#include <memory>
#include <string>

struct Material {
    std::string name;
    glm::vec3 ka;
    glm::vec3 kd;
    glm::vec3 ks;
    float shiny{0.0f};

    GLuint kaMap{0};
    GLuint kdMap{0};
    bool hasKaMap{false};
    bool hasKdMap{false};
    std::string kaMapPath;
    std::string kdMapPath;

    void putYourself(std::shared_ptr<Shader> &shader);
};


#endif //PA2_MATERIAL_H
