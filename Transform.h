#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <memory>
#include <unordered_set>
#include <glm/vec3.hpp>
#include "Node.h"

class Transform : public Node {
public:
    explicit Transform(const glm::mat4 &M);

    Transform() = default;

    void draw(const glm::mat4 &parent,
              const glm::mat4 &view,
              const glm::mat4 &projection) override;

    void update(std::chrono::milliseconds delta) override;

    std::list<Node *> hit(const Ray &ray) override;

    void addChild(Node *node);

    void removeChild(Node *node);

    void translate(const glm::vec3 &direction);

    void rotate(float angle, const glm::vec3 &axis);

    void scale(const glm::vec3 &mag);

    void position(glm::vec3 position);

    glm::vec3 position() const;

protected:
    glm::mat4 M;
    glm::mat4 base;
    glm::mat4 rotation, translation, scaletion;
    std::unordered_set<Node *> children;

    void updateM();
};

#endif // TRANSFORM_H