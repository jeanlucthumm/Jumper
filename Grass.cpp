#include "Grass.hpp"

Grass::Grass(MeshBank::refID pointLightID, MeshBank::refID segmentID,
             std::shared_ptr<Shader> lightShader, std::shared_ptr<Shader> segmentShader)
        : light{new PointLightObj{
        pointLightID, std::move(lightShader),
        0, 0.0, 0.05, 0.0,
        glm::vec3{0.3, 0.3, 0.3},
        glm::vec3{0.3, 0.3, 0.3},
        glm::vec3{0.3, 0.3, 0.3},}},
          segment{new Geometry{segmentID, std::move(segmentShader)}},
          lightTrans{new Transform}, segTrans{new Transform} {
    lightTrans->addChild(light);
    segTrans->addChild(segment);
    addChild(lightTrans);
    addChild(segTrans);

    segTrans->scale(glm::vec3{5});

    lightTrans->translate(glm::vec3{0, 1, 0});
}

void Grass::attach(const std::shared_ptr<Shader> &shader) {
    light->attach(shader);
}

void Grass::detach(const std::shared_ptr<Shader> &shader) {
    light->detach(shader);
}
