//
// Created by Jean-Luc Thumm on 12/9/17.
//

#include "PointLight.hpp"

PointLight::PointLight(unsigned int shaderArrayPos,
                       float constant, float linear, float quadratic,
                       glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
        : constant{constant}, linear{linear}, quadratic{quadratic},
          ambient{ambient}, diffuse{diffuse}, specular{specular} {
    tag = std::string{GLSL_ARRAY_NAME} + "[" + std::to_string(shaderArrayPos) + "]";
}

void PointLight::attach(const std::shared_ptr<Shader> &shader) {
    shaders.insert(shader);
}

void PointLight::detach(const std::shared_ptr<Shader> &shader) {
    shaders.erase(shader);
}

void PointLight::draw(const glm::mat4 &parent, const glm::mat4 &view, const glm::mat4 &projection) {
    glm::vec3 position = glm::vec3{parent * glm::vec4{0, 0, 0, 1}};

    for (auto &shader : shaders) {
        shader->use();
        shader->put(tag + ".position", position);
        shader->put(tag + ".constant", constant);
        shader->put(tag + ".linear", linear);
        shader->put(tag + ".quadratic", quadratic);
        shader->put(tag + ".ambient", ambient);
        shader->put(tag + ".diffuse", diffuse);
        shader->put(tag + ".specular", specular);
    }
}

void PointLight::update() {}

std::list<Node *> PointLight::hit(const Ray &ray) { return std::list<Node *>{}; }
