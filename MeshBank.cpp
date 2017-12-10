//
// Created by Jean-Luc Thumm on 12/7/17.
//

#include "MeshBank.h"
#include "MaterialBank.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/geometric.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
namespace alg = boost::algorithm;

std::vector<const OBJElement> MeshBank::table;

const std::vector<const OBJElement> &MeshBank::get(refID objId) {
    if (table.empty() || objId > table.size() - 1) {
        throw std::runtime_error{"OBJBank could not find objID"};
    }
    return table[objId];
}

MeshBank::refID MeshBank::load(std::string path) {
    std::ifstream in{path};
    if (!in.is_open()) {
        std::cerr << "Could not open file: " << path
                  << ": " << std::strerror(errno) << std::endl;
        return 0;
    }
    fs::path bpath{fs::path{path}.parent_path()};

    std::vector<const OBJElement> object;

    std::string line, token;
    float x, y, z;

    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    minX = minY = minZ = std::numeric_limits<float>::max();
    maxX = maxY = maxZ = std::numeric_limits<float>::min();

    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec3> tempUvs;
    std::vector<std::string> splitRes;    // face line tokens

    const OBJElement element;

    while (getline(in, line)) {
        std::stringstream ss{line};
        ss >> token;

        // TODO handle case of no 'o' command

        if (token == "mtllib") {
            ss >> token;
            fs::path p = bpath / fs::path{token};
            if (!MaterialBank::I()->parse(p.string())) {
                std::cerr << "Could not parse mtllib:" << p.string() << std::endl;
            }
        }
        else if (token == "o") {
//            table.push_back(element);
        }
        else if (token == "v") {
            ss >> x;
            ss >> y;
            ss >> z;

            if (x < minX) minX = x;
            else if (x > maxX) maxX = x;
            if (y < minY) minY = y;
            else if (y > maxY) maxY = y;
            if (z < minZ) minZ = z;
            else if (z > maxZ) maxZ = z;

            element.vertices.emplace_back(x, y, z);
        }
        else if (token == "vn") {
            ss >> x;
            ss >> y;
            ss >> z;
            glm::vec3 normal{x, y, z};

            tempNormals.push_back(glm::normalize(normal));
        }
        else if (token == "f") {
            // make sure we have enough room to align
            if (element.normals.size() < element.vertices.size()) {
                element.normals.resize(element.vertices.size());
            }
            if (element.uvs.size() < element.uvs.size()) {
                element.uvs.resize(element.vertices.size());
            }

            for (int i = 0; i < 3; i++) {
                // split by /
                ss >> token;
                splitRes.clear();
                alg::split(splitRes, token, alg::is_any_of("/"));
                if (splitRes.size() != 3) continue; // f xx/xx/xx

                // populate arrays
                unsigned long index = std::stoul(splitRes[0]) - 1;
                element.indices.push_back(index);
                if (!splitRes[1].empty()) {
                    unsigned long uvIndex = std::stoul(splitRes[1]) - 1;
                    element.uvs[index] = tempUvs[uvIndex];
                }
                if (!splitRes[2].empty()) {
                    unsigned long normIndex = std::stoul(splitRes[2]) - 1;
                    element.normals[index] = tempNormals[normIndex];
                }
            }

            // check for ngon
            if (!ss.eof()) {
                std::cerr << "Found ngon in file: " << path;
                std::cerr << ": " << line << std::endl;
            }
        }
        else if (token == "usemtl") {
            ss >> token;
            element.material = MaterialBank::I()->get(token);
        }
    }

    element.max = glm::vec3{maxX, maxY, maxZ};
    element.min = glm::vec3{minX, minY, minZ};

    table.push_back(std::move(element));
    return table.size() - 1;
}
