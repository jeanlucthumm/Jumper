//
// Created by Jean-Luc Thumm on 11/12/17.
//

#ifndef PA2_CAMERA_H
#define PA2_CAMERA_H


#include <glm/vec3.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>

class Camera {
private:
    glm::vec3 pos;
    glm::vec3 lookat;
    glm::vec3 up;

    glm::mat4 view;
    glm::mat4 rotation;
    glm::mat4 rotationState;

    void updateView();

public:
    Camera(const glm::vec3 &pos,
           const glm::vec3 &lookat,
           const glm::vec3 &up);

    const glm::mat4 &viewMatrix();

    glm::vec3 position();

    void position(glm::vec3 pos);

    void save();

    void statefulRotate(float angle, const glm::vec3 &axis);

    void reset();
};


#endif //PA2_CAMERA_H
