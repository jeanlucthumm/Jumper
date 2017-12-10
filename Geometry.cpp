#include "Geometry.h"
#include "Window.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

Geometry::Geometry(MeshBank::refID dataID, std::shared_ptr<Shader> shader,
                   std::shared_ptr<Material> material)
        : shader{std::move(shader)}, dataID{dataID},
          data{MeshBank::I()->get(dataID)}, material{std::move(material)} {

    // TODO base matrix calculation
//    glm::vec3 center = (data.max + data.min) / 2.0f;
//    glm::mat4 T = glm::translate(-center);
//
//    glm::vec3 rangeVec = (data.max - data.min) / 2.0f;
//    float range = std::max({rangeVec.x, rangeVec.y, rangeVec.z});
//    glm::mat4 S = glm::scale((1.0f / range) * glm::vec3{1.0f});
//
//    base = S * T;
}

void Geometry::draw(const glm::mat4 &parent,
                    const glm::mat4 &view,
                    const glm::mat4 &projection) {
    shader->use();
    shader->put("model", parent * base);
    shader->put("view", view);
    shader->put("projection", projection);
    shader->put("cameraPos", Window::Instance().CameraPos());

    material->putYourself(shader);

//    glBindVertexArray(VAO);
//    glBindTexture(GL_TEXTURE_CUBE_MAP, Window::Instance().SkyboxTexture());
//    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(data..size()),
//                   GL_UNSIGNED_INT, nullptr);
//    glBindVertexArray(0);
}

void Geometry::update() {

}

std::list<Node *> Geometry::hit(const Ray &ray) {
    return std::list<Node *>{};
}

