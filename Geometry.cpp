#include "Geometry.h"
#include "Window.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

#include "debug.h"

Geometry::Geometry(MeshBank::refID dataID, std::shared_ptr<Shader> shader,
                   std::shared_ptr<Material> material)
        : shader{std::move(shader)}, dataID{dataID},
          data{MeshBank::I()->get(dataID)}, material{std::move(material)} {

    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    minX = minY = minZ = std::numeric_limits<float>::max();
    maxX = maxY = maxZ = std::numeric_limits<float>::min();

    for (auto &element : data) {
        minX = std::min(minX, element.min.x);
        minY = std::min(minY, element.min.y);
        minZ = std::min(minZ, element.min.z);

        maxX = std::max(maxX, element.max.x);
        maxY = std::max(maxY, element.max.y);
        maxZ = std::max(maxZ, element.max.z);
    }

    glm::vec3 min{minX, minY, minZ};
    glm::vec3 max{maxX, maxY, maxZ};

    glm::vec3 center = (max + min) / 2.0f;
    glm::mat4 T = glm::translate(-center);

    glm::vec3 rangeVec = (max - min) / 2.0f;
    float range = std::max({rangeVec.x, rangeVec.y, rangeVec.z});
    glm::mat4 S = glm::scale((1.0f / range) * glm::vec3{1.0f});

    base = S * T;
}

void Geometry::draw(const glm::mat4 &parent,
                    const glm::mat4 &view,
                    const glm::mat4 &projection) {
    shader->use();
    shader->put("model", parent * base);
    shader->put("view", view);
    shader->put("projection", projection);
    shader->put("cameraPos", Window::Instance().CameraPos());

    for (auto &element: data) {
        put(element.material);
        glBindVertexArray(element.VAO);

        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(element.vertices.size()),
                       GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
}

void Geometry::update() {

}

std::list<Node *> Geometry::hit(const Ray &ray) {
    return std::list<Node *>{};
}

void Geometry::put(const std::shared_ptr<const Material> &material) {
    shader->put("material.ambient", material->ka);
    shader->put("material.diffuse", material->kd);
    shader->put("material.specular", material->ks);
    shader->put("material.shiny", material->shiny);
}

