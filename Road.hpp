#ifndef JUMPER_ROAD_H
#define JUMPER_ROAD_H

#include "Transform.h"
#include "Geometry.h"
#include "MeshBank.h"

class Road : public Transform {
public:
    Road(MeshBank::refID carID, MeshBank::refID segmentID,
         std::shared_ptr<Shader> carShader, std::shared_ptr<Shader> segmentShader);

    void draw(const glm::mat4 &parent,
              const glm::mat4 &view,
              const glm::mat4 &projection) override;

    void update(std::chrono::milliseconds delta) override;

    std::list<Node *> hit(const Ray &ray) override;

    const Geometry *Car() const;

private:
    Geometry *car;
    Geometry *segment;
    Transform *carTrans;
    Transform *segTrans;

    bool left; // true if car coming from left
};

#endif