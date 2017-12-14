#ifndef JUMPER_POINTLIGHTOBJ_H
#define JUMPER_POINTLIGHTOBJ_H

#include "Geometry.h"
#include "Transform.h"
#include "PointLight.hpp"
#include "MeshBank.h"
#include <unordered_set>

class PointLightObj : public Transform {
public:
    PointLightObj(MeshBank::refID sphereID, std::shared_ptr<Shader> shader,
                      unsigned int shaderArrayPos, float constant, float linear,
                      float quadratic, glm::vec3 ambient, glm::vec3 diffuse,
                      glm::vec3 specular);

    void attach(const std::shared_ptr<Shader> &shader);
    void detach(const std::shared_ptr<Shader> &shader);

private:
    Geometry *obj;
    PointLight *light;
    Transform *trans;
};

#endif
