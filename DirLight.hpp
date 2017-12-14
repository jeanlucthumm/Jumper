//
// Created by Jean-Luc Thumm on 12/9/17.
//

#ifndef JUMPER_DIRLIGHT_HPP
#define JUMPER_DIRLIGHT_HPP

#include "Node.h"
#include "Shader.h"
#include <glm/vec3.hpp>
#include <memory>
#include <unordered_set>
#include <string>

#define GLSL_ARRAY_NAME "dirLights"

class DirLight : public Node {
public:

    DirLight(glm::vec3 direction,
             glm::vec3 color,
             size_t shaderArrayPos);

    void attach(const std::shared_ptr<Shader> &shader);
    void detach(const std::shared_ptr<Shader> &shader);

    void draw(const glm::mat4 &parent, const glm::mat4 &view,
              const glm::mat4 &projection) override;
    void update(std::chrono::milliseconds delta) override;
    std::list<Node *> hit(const Ray &ray) override;

private:
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    std::string tag;

    std::unordered_set<std::shared_ptr<Shader>> shaders;
};

#endif //JUMPER_DIRLIGHT_HPP
