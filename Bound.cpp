#include "Bound.hpp"

Bound::Bound(glm::vec3 min, glm::vec3 max)
        : min{min}, max{max} {}

bool Bound::intersect(const Bound &other) const {
    if (max.x < other.min.x || min.x > other.max.x) return false;
    if (max.y < other.min.y || min.y > other.max.y) return false;
    if (max.z < other.min.z || min.z > other.max.z) return false;

    return true;
}

std::vector<glm::vec3> Bound::GLpoints() const {
    std::vector<glm::vec3> ret;

    ret.push_back(min);
    ret.push_back(glm::vec3{max.x, min.y, min.z});
    ret.push_back(glm::vec3{max.x, min.y, min.z});
    ret.push_back(glm::vec3{max.x, max.y, min.z});
    ret.push_back(glm::vec3{max.x, max.y, min.z});
    ret.push_back(glm::vec3{min.x, max.y, min.z});
    ret.push_back(glm::vec3{min.x, max.y, min.z});
    ret.push_back(min);

    ret.push_back(max);
    ret.push_back(glm::vec3{max.x, min.y, max.z});
    ret.push_back(glm::vec3{max.x, min.y, max.z});
    ret.push_back(glm::vec3{min.x, min.y, max.z});
    ret.push_back(glm::vec3{min.x, min.y, max.z});
    ret.push_back(glm::vec3{min.x, max.y, max.z});
    ret.push_back(glm::vec3{min.x, max.y, max.z});
    ret.push_back(max);

    ret.push_back(min);
    ret.push_back(glm::vec3{min.x, min.y, max.z});
    ret.push_back(glm::vec3{max.x, min.y, min.z});
    ret.push_back(glm::vec3{max.x, min.y, max.z});
    ret.push_back(glm::vec3{max.x, max.y, min.z});
    ret.push_back(glm::vec3{max.x, max.y, max.z});
    ret.push_back(glm::vec3{min.x, max.y, min.z});
    ret.push_back(glm::vec3{min.x, max.y, max.z});

    return ret;
}

Bound Bound::align(const glm::mat4 &transformation) const {
    auto corns = corners();
    for (auto &corner : corns) {
        corner = glm::vec3{transformation * glm::vec4{corner, 1}};
    }

    glm::vec3 amin, amax;
    amin.x = amin.y = amin.z = std::numeric_limits<float>::max();
    amax.x = amax.y = amax.z = std::numeric_limits<float>::min();

    for (auto &corner : corns) {
        amin.x = std::min(amin.x, corner.x);
        amin.y = std::min(amin.y, corner.y);
        amin.z = std::min(amin.z, corner.z);

        amax.x = std::max(amax.x, corner.x);
        amax.y = std::max(amax.y, corner.y);
        amax.z = std::max(amax.z, corner.z);
    }

    return Bound(amin, amax);
}

std::vector<glm::vec3> Bound::corners() const {
    std::vector<glm::vec3> ret;

    ret.push_back(min);

    ret.push_back(glm::vec3{max.x, min.y, min.z});
    ret.push_back(glm::vec3{min.x, max.y, min.z});
    ret.push_back(glm::vec3{min.x, min.y, max.z});

    ret.push_back(glm::vec3{min.x, max.y, max.z});
    ret.push_back(glm::vec3{max.x, min.y, max.z});
    ret.push_back(glm::vec3{max.x, max.y, min.z});

    ret.push_back(max);

    return ret;
}

#pragma clang diagnostic pop