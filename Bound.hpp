#ifndef JUMPER_BOUND_H
#define JUMPER_BOUND_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

class Bound {
public:
    Bound(glm::vec3 min, glm::vec3 max);
    Bound(const Bound &other) = default;

    void transform(const glm::mat4 &transformation);
    void grow(const Bound &other);
    void grow(const Bound &other,
                   const glm::mat4 &transformation);
    bool intersect(const Bound &other) const;
    bool intersect(const Bound &other,
                   const glm::mat4 &transformation) const;
    bool contains(glm::vec3 point) const;
    std::vector<glm::vec3> points() const;

    glm::vec3 Min() const;
    glm::vec3 Max() const;

private:
    glm::vec3 min;
    glm::vec3 max;
};

#endif
