#ifndef JUMPER_GRASS_H
#define JUMPER_GRASS_H

#include "Transform.h"
#include "Shader.h"
#include "MeshBank.h"
#include "PointLightObj.hpp"
#include <memory>

class Grass : public Transform {
public:
    Grass(MeshBank::refID pointLightID, MeshBank::refID segmentID,
          std::shared_ptr<Shader> lightShader, std::shared_ptr<Shader> segmentShader);

    void attach(const std::shared_ptr<Shader> &shader);
    void detach(const std::shared_ptr<Shader> &shader);

private:
    PointLightObj *light;
    Geometry *segment;
    Transform *lightTrans;
    Transform *segTrans;
};

#endif
