#include "Transform.h"
#include "Road.hpp"
#include <glm/glm.hpp>

void Road::draw(const glm::mat4 &parent, const glm::mat4 &view, const glm::mat4 &projection) {
    Transform::draw(parent, view, projection);
}

void Road::update() {
    Transform::update();

    carTrans->translate(glm::vec3{0.04, 0, 0});
    if (carTrans->position().x > 5) {
        carTrans->translate(glm::vec3{-10, 0, 0});
    }
}

std::list<Node *> Road::hit(const Ray &ray) {
    return Transform::hit(ray);
}

const Geometry *Road::Car() const {
    return car;
}

Road::Road(MeshBank::refID carID, MeshBank::refID segmentID, std::shared_ptr<Shader> carShader,
           std::shared_ptr<Shader> segmentShader)
        : car{new Geometry{carID, std::move(carShader)}},
          segment{new Geometry{segmentID, std::move(segmentShader)}},
          carTrans{new Transform}, segTrans{new Transform} {
    carTrans->addChild(car);
    segTrans->addChild(segment);
    addChild(carTrans);
    addChild(segTrans);

    carTrans->rotate(90.0f, glm::vec3{0.0f, 0.1f, 0.0f});
    carTrans->translate(glm::vec3{0, 0.3, 0});

    carTrans->translate(glm::vec3{-5, 0, 0});

    segTrans->scale(glm::vec3{5});
}
