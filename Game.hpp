#ifndef JUMPER_GAME_H
#define JUMPER_GAME_H

#include "Transform.h"
#include "Geometry.h"

class Game : public Transform {
public:
    Game(MeshBank::refID carID, MeshBank::refID grassID, MeshBank::refID cubeID,
             MeshBank::refID roadID, std::shared_ptr<Shader> standard,
             std::shared_ptr<Shader> light);

    static constexpr float WIDTH = 1.2f;

private:
    std::vector<Geometry *> cars;

    Geometry *player;
    Transform *playerTrans;
};

#endif
