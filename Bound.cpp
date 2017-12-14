#include "Bound.hpp"
#include <algorithm>

Bound::Bound(glm::vec3 min, glm::vec3 max)
        : min{min}, max{max} {}

void Bound::transform(const glm::mat4 &transformation) {
    min = glm::vec3{transformation * glm::vec4{min, 1}};
    max = glm::vec3{transformation * glm::vec4{max, 1}};
}

void Bound::grow(const Bound &other) {
    min.x = std::min(min.x, other.min.x);
    min.y = std::min(min.y, other.min.y);
    min.z = std::min(min.z, other.min.z);

    max.x = std::max(max.x, other.max.x);
    max.y = std::max(max.y, other.max.y);
    max.z = std::max(max.z, other.max.z);
}

void Bound::grow(const Bound &other, const glm::mat4 &transformation) {
    glm::vec3 omin{transformation * glm::vec4{other.min, 1}};
    glm::vec3 omax{transformation * glm::vec4{other.max, 1}};

    min.x = std::min(min.x, omin.x);
    min.y = std::min(min.y, omin.y);
    min.z = std::min(min.z, omin.z);

    max.x = std::max(max.x, omax.x);
    max.y = std::max(max.y, omax.y);
    max.z = std::max(max.z, omax.z);
}

bool Bound::intersect(const Bound &other) const {
    bool detected = false;
    detected |= contains(other.min);
    detected |= contains(other.max);
    detected |= contains(glm::vec3{other.max.x, other.min.y, other.min.z});
    detected |= contains(glm::vec3{other.min.x, other.max.y, other.min.z});
    detected |= contains(glm::vec3{other.min.x, other.min.y, other.max.z});
    detected |= contains(glm::vec3{other.min.x, other.max.y, other.max.z});
    detected |= contains(glm::vec3{other.max.x, other.min.y, other.max.z});
    detected |= contains(glm::vec3{other.max.x, other.max.y, other.min.z});
    return detected;
}

bool Bound::intersect(const Bound &other, const glm::mat4 &transformation) const {
    glm::vec3 omin{transformation * glm::vec4{other.min, 1}};
    glm::vec3 omax{transformation * glm::vec4{other.max, 1}};

    bool detected = false;
    detected |= contains(other.min);
    detected |= contains(other.max);
    detected |= contains(glm::vec3{omax.x, omin.y, omin.z});
    detected |= contains(glm::vec3{omin.x, omax.y, omin.z});
    detected |= contains(glm::vec3{omin.x, omin.y, omax.z});
    detected |= contains(glm::vec3{omin.x, omax.y, omax.z});
    detected |= contains(glm::vec3{omax.x, omin.y, omax.z});
    detected |= contains(glm::vec3{omax.x, omax.y, omin.z});
    return detected;
}

glm::vec3 Bound::Min() const {
    return glm::vec3();
}

glm::vec3 Bound::Max() const {
    return glm::vec3();
}

bool Bound::contains(glm::vec3 point) const {
    return min.x <= point.x && point.x <= max.x
           && min.y <= point.y && point.y <= max.y
           && min.z <= point.z && point.z <= max.z;
}

std::vector<glm::vec3> Bound::points() const {
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

