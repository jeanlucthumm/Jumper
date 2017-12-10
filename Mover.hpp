//
// Created by Jean-Luc Thumm on 12/9/17.
//

#ifndef JUMPER_MOVER_HPP
#define JUMPER_MOVER_HPP

#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"

class Mover : public Transform {
public:
    void update() override {
        Transform::update();
        translate(glm::vec3(-0.01f, 0.00f, 0.0f));
    }
};

#endif //JUMPER_MOVER_HPP
