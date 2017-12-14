#include "Transform.h"
#include "Road.hpp"
#include <glm/glm.hpp>
#include <random>

void Road::draw(const glm::mat4 &parent, const glm::mat4 &view, const glm::mat4 &projection) {
    Transform::draw(parent, view, projection);
}

void Road::update(std::chrono::milliseconds delta) {
    Transform::update(delta);

    float millis = delta.count();
    float distance = CAR_SPEED * (millis / 1000.0f);

    if (left) {
        carTrans->translate(glm::vec3{-distance, 0, 0});
        if (carTrans->position().x < -5) {
            carTrans->translate(glm::vec3{10, 0, 0});
        }
    }
    else {
        carTrans->translate(glm::vec3{distance, 0, 0});
        if (carTrans->position().x > 5) {
            carTrans->translate(glm::vec3{-10, 0, 0});
        }
    }
}

std::list<Node *> Road::hit(const Ray &ray) {
    return Transform::hit(ray);
}

const Geometry *Road::Car() const {
    return car;
}

#include "debug.h"

Road::Road(MeshBank::refID carID, MeshBank::refID segmentID, std::shared_ptr<Shader> carShader,
           std::shared_ptr<Shader> segmentShader)
        : car{new Geometry{carID, std::move(carShader)}},
          segment{new Geometry{segmentID, std::move(segmentShader)}},
          carTrans{new Transform}, segTrans{new Transform} {
    carTrans->addChild(car);
    segTrans->addChild(segment);
    addChild(carTrans);
    addChild(segTrans);

    // determine left or right and starting point
    left = static_cast<bool>(rand() % 2);

    float pos = -4.0f + (8.0f / 100.0f) * static_cast<float>(rand() % 100);

    carTrans->translate(glm::vec3{0, 0.3, 0});
    segTrans->scale(glm::vec3{5});
    carTrans->translate(glm::vec3{pos, 0, 0});
    if (left) {
        carTrans->rotate(-90.0f, glm::vec3{0.0f, 0.1f, 0.0f});
    }
    else {
        carTrans->rotate(90.0f, glm::vec3{0.0f, 0.1f, 0.0f});
    }

}
