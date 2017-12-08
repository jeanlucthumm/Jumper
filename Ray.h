//
// Created by Jean-Luc Thumm on 11/26/17.
//

#ifndef PA4_RAY_H
#define PA4_RAY_H

#include <glm/vec3.hpp>
#include <utility>

class Ray {
public:
    Ray(glm::vec3 origin, glm::vec3 direction);

    glm::vec3 Origin() const;
    glm::vec3 Direction() const;

    glm::vec3 value(float t) const;

    std::pair<bool, std::pair<float, float>>
    intersectSphere(glm::vec3 center, float radius) const;

private:
    glm::vec3 origin;
    glm::vec3 direction;
};


#endif //PA4_RAY_H
