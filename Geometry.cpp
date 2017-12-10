#include "Geometry.h"
#include "Window.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

Geometry::Geometry(OBJBank::refID dataID, std::shared_ptr<Shader> shader,
                   std::shared_ptr<Material> material)
        : shader{std::move(shader)}, dataID{dataID},
          data{OBJBank::get(dataID)}, material{std::move(material)} {

    // base matrix calculation
    glm::vec3 center = (data.max + data.min) / 2.0f;
    glm::mat4 T = glm::translate(-center);

    glm::vec3 rangeVec = (data.max - data.min) / 2.0f;
    float range = std::max({rangeVec.x, rangeVec.y, rangeVec.z});
    glm::mat4 S = glm::scale((1.0f / range) * glm::vec3{1.0f});

    base = S * T;

    // OpenGL
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VVBO);
    glGenBuffers(1, &NVBO);
    glBindVertexArray(VAO);

    // positions
    glBindBuffer(GL_ARRAY_BUFFER, VVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.vertices.size()) * sizeof(glm::vec3),
                 &data.vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    // normals
    glBindBuffer(GL_ARRAY_BUFFER, NVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.normals.size()) * sizeof(glm::vec3),
                 &data.normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(data.faces.size()) * sizeof(unsigned int),
                 &data.faces[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Geometry::draw(const glm::mat4 &parent,
                    const glm::mat4 &view,
                    const glm::mat4 &projection) {
    shader->use();
    shader->put("model", parent);
    shader->put("view", view);
    shader->put("projection", projection);
    shader->put("cameraPos", Window::Instance().CameraPos());

    material->putYourself(shader);

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Window::Instance().SkyboxTexture());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(data.faces.size()),
                   GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Geometry::update() {

}

std::list<Node *> Geometry::hit(const Ray &ray) {
    return std::list<Node *>{};
}

