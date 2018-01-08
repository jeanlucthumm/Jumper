
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

    std::vector<glm::vec3> vertices = bound.GLpoints();

    makeVAO(vertices);

    initialized = true;
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
    initialized = false;
}

void BoundObj::setBound(const Bound &bound) {
    this->bound = bound;
    std::vector<glm::vec3> vertices = bound.GLpoints();

    if (initialized) {
        auto size = static_cast<GLsizeiptr>(vertices.size());
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, &vertices[0]);
        glBindVertexArray(0);
    }
    else {
        makeVAO(vertices);
    }
}

void BoundObj::makeVAO(const std::vector<glm::vec3> &vertices) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    auto vsize = static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec3));

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vsize, &vertices[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    initialized = true;
}
