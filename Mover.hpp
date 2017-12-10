//
// Created by Jean-Luc Thumm on 12/9/17.
//

#ifndef JUMPER_MOVER_HPP
#define JUMPER_MOVER_HPP

#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"
#include "debug.h"

class Mover : public Transform {
public:
    void update() override {
        Transform::update();
        cout << "Update" << endl;// DEBUG
        M = glm::rotate(M, glm::radians(1.0f), glm::vec3{0, 0, 1});
    }
};

#endif //JUMPER_MOVER_HPP
