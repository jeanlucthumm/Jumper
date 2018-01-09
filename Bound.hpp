#ifndef JUMPER_BOUND_H
#define JUMPER_BOUND_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

class Bound {
public:
    Bound() = default;
    Bound(glm::vec3 min, glm::vec3 max);

    Bound align(const glm::mat4 &transformation) const;
    /// \pre Both bounds are axis aligned
    bool intersect(const Bound &other) const;
    std::vector<glm::vec3> GLpoints() const;
    std::vector<glm::vec3> corners() const;

    glm::vec3 Min() const;
    glm::vec3 Max() const;

private:
    glm::vec3 min, max;
};

#endif
