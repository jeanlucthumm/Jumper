#include "Transform.h"
#include "Road.hpp"

void Road::draw(const glm::mat4 &parent, const glm::mat4 &view, const glm::mat4 &projection) {
    Transform::draw(parent, view, projection);
}

void Road::update() {
    Transform::update();
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
}
