//
// Created by Jean-Luc Thumm on 11/12/17.
//

#include <glm/gtx/rotate_vector.hpp>
#include "Camera.h"

Camera::Camera(const glm::vec3 &pos, const glm::vec3 &lookat, const glm::vec3 &up)
        : pos{pos}, lookat{lookat}, up{up} {
    updateView();
}

const glm::mat4 &Camera::viewMatrix() {
    return view;
}

glm::vec3 Camera::position() {
    return glm::vec3{rotation * glm::vec4{pos, 1}};
}

void Camera::save() {
    rotationState = rotation;
}

void Camera::statefulRotate(float angle, const glm::vec3 &axis) {
    glm::mat4 newRotation = glm::rotate(angle, axis);
    rotation = rotationState * newRotation;
    updateView();
}

void Camera::updateView() {

    view = glm::lookAt(
            glm::vec3{rotation * glm::vec4{pos, 1}},
            lookat,
            glm::vec3{rotation * glm::vec4{up, 1}}
    );
}

void Camera::reset() {
    rotation = rotationState = glm::mat4{};
    updateView();
}

void Camera::position(glm::vec3 pos) {
    this->pos = pos;
    rotation = rotationState = glm::mat4{};
    updateView();
}
