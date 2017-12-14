#include "PointLightObj.hpp"

PointLightObj::PointLightObj(MeshBank::refID sphereID, std::shared_ptr<Shader> shader,
                             unsigned int shaderArrayPos, float constant, float linear,
                             float quadratic, glm::vec3 ambient, glm::vec3 diffuse,
                             glm::vec3 specular)
        : obj{new Geometry{sphereID, std::move(shader)}},
          light{new PointLight{
                  shaderArrayPos, constant, linear, quadratic, ambient, diffuse, specular
          }},
          trans{new Transform} {
    trans->scale(glm::vec3{0.06});
    trans->addChild(obj);
    trans->addChild(light);
    addChild(trans);
}

void PointLightObj::attach(const std::shared_ptr<Shader> &shader) {
    light->attach(shader);
}

void PointLightObj::detach(const std::shared_ptr<Shader> &shader) {
    light->detach(shader);
}
