//
// Created by Jean-Luc Thumm on 12/7/17.
//

#ifndef JUMPER_OBJBANK_H
#define JUMPER_OBJBANK_H

#include <string>
#include <vector>
#include <utility>
#include <glm/vec3.hpp>

class MeshBank {
public:
    typedef size_t refID;
    class Data;

    static refID load(std::string path);
    static const Data & get(refID objId);

    class Data {
    public:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> faces;
        glm::vec3 max;
        glm::vec3 min;
    };

private:
    static std::vector<Data> table;
};


#endif //JUMPER_OBJBANK_H
