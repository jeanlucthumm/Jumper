#include "Game.hpp"
#include "Grass.hpp"
#include "Road.hpp"
#include "Window.h"

Game::Game(MeshBank::refID carID, MeshBank::refID grassID, MeshBank::refID cubeID,
           MeshBank::refID roadID, std::shared_ptr<Shader> standard,
           std::shared_ptr<Shader> light)
        : size{0}, playerLoc{0} {
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
    size = counter;

    Window::I().subscribe(GLFW_KEY_SPACE, this);
}

void Game::receive(const Event &event) {}

void Game::receive(int key) {
    if (playerLoc < size - 1) {
        playerTrans->translate(glm::vec3{0, 0, -WIDTH});
        playerLoc++;
    }
    else {
        playerTrans->translate(glm::vec3{0, 0, WIDTH * (size - 1)});
        playerLoc = 0;
    }
}

void Game::update(std::chrono::milliseconds delta) {
    Transform::update(delta);

    Bound playerAABB = player->getBound().align(playerTrans->matrix());

    for (auto child : children) {
        child->hit(playerAABB, glm::mat4{});
    }
}
