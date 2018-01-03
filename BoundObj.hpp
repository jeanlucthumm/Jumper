#ifndef JUMPER_BOUNDOBJ_H
#define JUMPER_BOUNDOBJ_H

#include "Node.h"
#include "GLinclude.h"
#include "Shader.h"
#include "EventListener.h"
#include "Bound.hpp"
#include <chrono>
#include <glm/mat4x4.hpp>
#include <memory>

class BoundObj : public Node, public EventListener {
public:
    BoundObj(const Bound &bound, std::shared_ptr<Shader> colorShader);
    BoundObj(std::shared_ptr<Shader> colorShader);

    void draw(const glm::mat4 &parent,
              const glm::mat4 &view,
              const glm::mat4 &projection) override;
    void update(std::chrono::milliseconds delta) override;
    std::list<Node *> hit(const Ray &ray) override;

    void setActive();
    void setPassive();

    void receive(const Event &event) override;
    void receive(int key) override;

    Bound getBound() const;
    void setBound(const Bound &);

private:
    GLuint VAO;
    bool enabled;
    glm::vec3 color;
    Bound bound;
    std::shared_ptr<Shader> shader;
    glm::mat4 lastModelMat;
};

#endif
