//
// Created by Jean-Luc Thumm on 12/9/17.
//
#include <glm/gtc/matrix_transform.hpp>
#include "DirLight.hpp"

void DirLight::attach(const std::shared_ptr<Shader> &shader) {
    shaders.insert(shader);
}

void DirLight::detach(const std::shared_ptr<Shader> &shader) {
    shaders.erase(shader);
}

void DirLight::draw(const glm::mat4 &parent, const glm::mat4 &view,
                    const glm::mat4 &projection) {
    for (auto &shader : shaders) {
        shader->use();
        shader->put(tag + ".direction", direction);
        shader->put(tag + ".ambient", ambient);
        shader->put(tag + ".diffuse", diffuse);
        shader->put(tag + ".specular", specular);
    }
}

void DirLight::update() {
};

std::list<Node *> DirLight::hit(const Ray &ray) {
    return std::list<Node *>{};
}

DirLight::DirLight(glm::vec3 direction, glm::vec3 color, size_t shaderArrayPos)
        : direction{direction}, ambient{color}, diffuse{color}, specular{color} {
    tag = std::string{GLSL_ARRAY_NAME} + "[" + std::to_string(shaderArrayPos) + "]";
}
