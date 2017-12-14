#include "Grass.hpp"

Grass::Grass(MeshBank::refID pointLightID, MeshBank::refID segmentID,
             std::shared_ptr<Shader> lightShader, std::shared_ptr<Shader> segmentShader,
             unsigned int light1Pos,
             unsigned int light2Pos)
        : light1{new PointLightObj{
        pointLightID, lightShader,
        light1Pos, 0.0, 0.05, 0.0,
        glm::vec3{0.3, 0.3, 0.3},
        glm::vec3{0.3, 0.3, 0.3},
        glm::vec3{0.3, 0.3, 0.3},}},
          light2{new PointLightObj{
                  pointLightID, lightShader,
                  light2Pos, 0.0, 0.05, 0.0,
                  glm::vec3{0.3, 0.3, 0.3},
                  glm::vec3{0.3, 0.3, 0.3},
                  glm::vec3{0.3, 0.3, 0.3},}},
          segment{new Geometry{segmentID, std::move(segmentShader)}},
          segTrans{new Transform} {
    segTrans->addChild(segment);
    addChild(segTrans);
    addChild(light1);
    addChild(light2);

    segTrans->scale(glm::vec3{5});

    light1->translate(glm::vec3{2.5, 1, 0});
    light2->translate(glm::vec3{-2.5, 1, 0});
}

void Grass::attach(const std::shared_ptr<Shader> &shader) {
    light1->attach(shader);
    light2->attach(shader);
}

void Grass::detach(const std::shared_ptr<Shader> &shader) {
    light1->detach(shader);
    light2->detach(shader);
}

void Grass::update(std::chrono::milliseconds delta) {
    Transform::update(delta);
}
