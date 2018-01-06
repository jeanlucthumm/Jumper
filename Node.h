#ifndef NODE_H
#define NODE_H

#include "Ray.h"
#include "Bound.hpp"
#include <glm/mat4x4.hpp>
#include <list>
#include <chrono>

class Node {
public:
    virtual void draw(const glm::mat4 &parent,
                      const glm::mat4 &view,
                      const glm::mat4 &projection) = 0;

    virtual void update(std::chrono::milliseconds delta) = 0;

    virtual std::list<Node *> hit(const Ray &ray) = 0;

    virtual std::list<Node *> hit(const Bound &bound,
                                  const glm::mat4 &parent) = 0;
};

#endif