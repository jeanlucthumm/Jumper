#ifndef JUMPER_GRASS_H
#define JUMPER_GRASS_H

#include "Transform.h"
#include "Shader.h"
#include "MeshBank.h"
#include "PointLightObj.hpp"
#include <memory>

class Grass : public Transform {
public:
    static constexpr float LIGHT_SPEED = 0.1f;

    Grass(MeshBank::refID pointLightID, MeshBank::refID segmentID,
          std::shared_ptr<Shader> lightShader, std::shared_ptr<Shader> segmentShader,
          unsigned int light1Pos,
          unsigned int light2Pos);

    void attach(const std::shared_ptr<Shader> &shader);
    void detach(const std::shared_ptr<Shader> &shader);

    void update(std::chrono::milliseconds delta) override;

private:
    PointLightObj *light1, *light2;
    Geometry *segment;
    Transform *segTrans;
};

#endif
