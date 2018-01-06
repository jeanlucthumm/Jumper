
#include "BoundObj.hpp"
#include "Window.h"

#include "debug.h""
#include "glutil.hpp"

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

BoundObj::BoundObj(const Bound &bound, std::shared_ptr<Shader> colorShader)
        : bound{bound}, shader{std::move(colorShader)}, enabled{false} {
    setPassive();
    Window::I().subscribe(GLFW_KEY_B, this);

    setBound(bound);
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

BoundObj::BoundObj(std::shared_ptr<Shader> colorShader)
        : bound{glm::vec3{}, glm::vec3{}},
          shader{std::move(colorShader)}, enabled{false} {
    setPassive();
    Window::I().subscribe(GLFW_KEY_B, this);
}

void BoundObj::setBound(const Bound &bound) {
    this->bound = bound;
    VAO = glutil::makeStdVAO(bound.GLpoints()); // TODO just realod the points
}
