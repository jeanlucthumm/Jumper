#include "Game.hpp"
#include "Grass.hpp"
#include "Road.hpp"

Game::Game(MeshBank::refID carID, MeshBank::refID grassID, MeshBank::refID cubeID,
           MeshBank::refID roadID, std::shared_ptr<Shader> standard,
           std::shared_ptr<Shader> light) {
    player = new Geometry{cubeID, standard};
    playerTrans = new Transform;
    playerTrans->addChild(player);
    addChild(playerTrans);


    Grass *grass;
    Road *road;

    grass = new Grass{cubeID, grassID, light, standard, 0, 1};
    grass->attach(standard);
    addChild(grass);

    road = new Road{carID, roadID, standard, standard};
    road->translate(glm::vec3{0, 0, WIDTH});
    addChild(road);
}
