//
// Created by Jean-Luc Thumm on 12/7/17.
//

#ifndef JUMPER_OBJBANK_H
#define JUMPER_OBJBANK_H

#include "GLinclude.h"
#include "OBJElement.hpp"
#include <string>
#include <vector>
#include <utility>
#include <glm/vec3.hpp>

class MeshBank {
public:
    typedef size_t refID;

    static MeshBank *I();
    static void instantiate();
    static void free();

    refID load(std::string path);
    const std::vector<OBJElement> &get(refID id);

private:
    static MeshBank *instance;

    MeshBank() = default;
    bool put(OBJElement &element);

    std::vector<std::vector<OBJElement>> table;
};


#endif //JUMPER_OBJBANK_H
