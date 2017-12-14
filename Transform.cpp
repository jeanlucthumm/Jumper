
#include <glm/gtx/transform.hpp>
#include "Transform.h"

Transform::Transform(const glm::mat4 &M) : base{M} {
    updateM();
}

void Transform::draw(const glm::mat4 &parent,
                     const glm::mat4 &view,
                     const glm::mat4 &projection) {
    glm::mat4 childMatrix = parent * M;
    for (auto child : children) {
        child->draw(childMatrix, view, projection);
    }
}

void Transform::update(std::chrono::milliseconds delta) {
    for (auto child : children) {
        child->update(delta);
    }
}

std::list<Node *> Transform::hit(const Ray &ray) {
    // TODO do bound test against itself first
    std::list<Node *> all;
    for (auto child : children) {
        all.splice(all.end(), child->hit(ray));
    }
    return all;
}

void Transform::addChild(Node *node) {
    children.insert(node);
}

void Transform::removeChild(Node *node) {
    children.erase(node);
}

void Transform::translate(const glm::vec3 &direction) {
    translation = glm::translate(translation, direction);
    updateM();
}

void Transform::rotate(float angle, const glm::vec3 &axis) {
    rotation = glm::rotate(rotation, glm::radians(angle), axis);
    updateM();
}

void Transform::scale(const glm::vec3 &mag) {
    scaletion = glm::scale(scaletion, mag);
    updateM();
}

void Transform::updateM() {
    M = base * translation * rotation * scaletion;
}

void Transform::position(glm::vec3 direction) {
    translation = glm::translate(direction);
    updateM();
}

glm::vec3 Transform::position() const {
    return glm::vec3{translation * glm::vec4{0, 0, 0, 1}};
}

