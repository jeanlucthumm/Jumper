
#include "BoundObj.hpp"
#include "Window.h"

#include "debug.h""

void BoundObj::draw(const glm::mat4 &parent, const glm::mat4 &view, const glm::mat4 &projection) {
    if (!enabled) return;

    shader->use();
    shader->put("color", color);
    shader->put("model", parent);
    shader->put("view", view);
    shader->put("projection", projection);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 24);
    glBindVertexArray(0);
}

void BoundObj::update(std::chrono::milliseconds delta) {

}

std::list<Node *> BoundObj::hit(const Ray &ray) {
    return std::list<Node *>{};
}

BoundObj::BoundObj(const Bound &bound, std::shared_ptr<Shader> colorShader)
        : bound{bound}, shader{std::move(colorShader)}, enabled{true} {
    setPassive();
    Window::Instance().subscribe(GLFW_KEY_B, this);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    auto points = bound.points();
    auto size = static_cast<GLsizeiptr>(points.size() * sizeof(glm::vec3));

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, &points[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void BoundObj::setActive() {
    color = glm::vec3{1, 0, 0};
}

void BoundObj::setPassive() {
    color = glm::vec3{0, 1, 0};
}

void BoundObj::receive(const Event &event) {}

void BoundObj::receive(int key) {
    enabled = !enabled;
}

const Bound & BoundObj::getBound() const {
    return bound;
}
