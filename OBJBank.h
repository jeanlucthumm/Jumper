//
// Created by Jean-Luc Thumm on 12/7/17.
//

#ifndef JUMPER_OBJBANK_H
#define JUMPER_OBJBANK_H

#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <unordered_map>

class OBJBank {
public:
    typedef int id;

    static id load(std::string path);

    static const std::vector<glm::vec3> &vertexData(id objId);
    static const std::vector<glm::vec3> &faceData(id objId);
    static const std::vector<glm::vec3> &normalData(id objId);

private:
    class MapData;

    static std::unordered_map<id, MapData> table;

    class MapData {
    public:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> faces;
        std::vector<glm::vec3> normals;
    };
};


#endif //JUMPER_OBJBANK_H
