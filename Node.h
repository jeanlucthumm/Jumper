#ifndef NODE_H
#define NODE_H

#include "Ray.h"
#include <glm/mat4x4.hpp>
#include <list>

class Node {
public:
    virtual void draw(const glm::mat4 &parent,
                      const glm::mat4 &view,
                      const glm::mat4 &projection) = 0;

    virtual void update() = 0;

    virtual std::list<Node *> hit(const Ray &ray) = 0;
};

#endif