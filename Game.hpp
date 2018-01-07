#ifndef JUMPER_GAME_H
#define JUMPER_GAME_H

#include "Transform.h"
#include "Geometry.h"

class Game : public Transform, EventListener {
public:
    Game(MeshBank::refID carID, MeshBank::refID grassID, MeshBank::refID cubeID,
             MeshBank::refID roadID, std::shared_ptr<Shader> standard,
             std::shared_ptr<Shader> light);

    static constexpr float WIDTH = 1.2f;

    void receive(const Event &event) override;
    void receive(int key) override;

    void update(std::chrono::milliseconds delta) override;

private:
    Geometry *player;
    Transform *playerTrans;
    int size;
    int playerLoc;
};

#endif
