//
// Created by Jean-Luc Thumm on 11/26/17.
//

#include <glm/geometric.hpp>
#include "Ray.h"

Ray::Ray(glm::vec3 origin, glm::vec3 direction)
    : origin{origin}, direction{glm::normalize(direction)} {}

std::pair<bool, std::pair<float, float>>
Ray::intersectSphere(glm::vec3 center, float radius) const {
    glm::vec3 diff = origin - center;
    float b = glm::dot(direction, diff);
    float c = glm::dot(diff, diff) - radius * radius;

    float discriminant = b * b - c;
    if (discriminant < 0) {
        return std::make_pair(false, std::make_pair(0, 0));
    }

    float t1 = -b - glm::sqrt(discriminant);
    float t2 = -b + glm::sqrt(discriminant);
    return std::make_pair(true, std::make_pair(t1, t2));
}
glm::vec3 Ray::Origin() const {
    return origin;
}
glm::vec3 Ray::Direction() const {
    return direction;
}

glm::vec3 Ray::value(float t) const {
    return origin + t * direction;
}
