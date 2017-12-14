#include "Game.hpp"
#include "Grass.hpp"
#include "Road.hpp"

Game::Game(MeshBank::refID carID, MeshBank::refID grassID, MeshBank::refID cubeID,
           MeshBank::refID roadID, std::shared_ptr<Shader> standard,
           std::shared_ptr<Shader> light) {
    player = new Geometry{cubeID, standard};
    playerTrans = new Transform;
    playerTrans->addChild(player);
    playerTrans->scale(glm::vec3{0.2f});
    playerTrans->translate(glm::vec3{0, 0.3f, 0});
    addChild(playerTrans);

    Grass *grass;
    Road *road;
    int counter = 0;
    int freeze;

    grass = new Grass{cubeID, grassID, light, standard, 0, 1};
    grass->attach(standard);
    grass->translate(glm::vec3{0, 0, -WIDTH * counter++});
    addChild(grass);

    road = new Road{carID, roadID, standard, standard};
    road->translate(glm::vec3{0, 0, -WIDTH * counter++});
    addChild(road);

    road = new Road{carID, roadID, standard, standard};
    road->translate(glm::vec3{0, 0, -WIDTH * counter++});
    addChild(road);

    grass = new Grass{cubeID, grassID, light, standard, 2, 3};
    grass->attach(standard);
    grass->translate(glm::vec3{0, 0, -WIDTH * counter++});
    addChild(grass);

    freeze = counter;
    for (; counter < freeze + 3; counter++) {
        road = new Road{carID, roadID, standard, standard};
        road->translate(glm::vec3{0, 0, -WIDTH * counter});
        addChild(road);
    }

    grass = new Grass{cubeID, grassID, light, standard, 4, 5};
    grass->attach(standard);
    grass->translate(glm::vec3{0, 0, -WIDTH * counter++});
    addChild(grass);

    translate(glm::vec3{0, 0, WIDTH * counter / 2});
}
