//
// Created by Jean-Luc Thumm on 12/9/17.
//

#ifndef JUMPER_POINTLIGHT_HPP
#define JUMPER_POINTLIGHT_HPP

#include "Geometry.h"
#include <unordered_set>

#define GLSL_ARRAY_NAME "pointLights"

class PointLight : public Geometry {
public:
    PointLight(OBJBank::refID objID,
               std::shared_ptr<Shader> shader,
               std::shared_ptr<Material> material,
               unsigned int shaderArrayPos,
               float constant, float linear, float quadratic,
               glm::vec3 ambient,
               glm::vec3 diffuse,
               glm::vec3 specular);

    void attach(const std::shared_ptr<Shader> &shader);
    void detach(const std::shared_ptr<Shader> &shader);

    void draw(const glm::mat4 &parent, const glm::mat4 &view,
              const glm::mat4 &projection) override;
    void update() override;
    std::list<Node *> hit(const Ray &ray) override;
private:
    float constant, linear, quadratic;
    glm::vec3 ambient, diffuse, specular;
    std::string tag;

    std::unordered_set<std::shared_ptr<Shader>> shaders;
};


#endif //JUMPER_POINTLIGHT_HPP
