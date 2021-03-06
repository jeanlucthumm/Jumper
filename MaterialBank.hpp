//
// Created by Jean-Luc Thumm on 12/10/17.
//

#ifndef JUMPER_MATERIALLOADER_HPP
#define JUMPER_MATERIALLOADER_HPP

#include "Material.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <boost/filesystem/path.hpp>

class MaterialBank {
public:
    static MaterialBank * I();
    static void instantiate();
    static void free();

    bool parse(const std::string &path);
    const std::shared_ptr<const Material> &get(const std::string &name);
    bool store(Material material);

private:
    static MaterialBank *instance;

    MaterialBank();
    GLuint loadTexture(const std::string &path);

    std::unordered_map<std::string, std::shared_ptr<const Material>> table;
    std::unordered_map<std::string, GLuint> textureTable;
};


#endif //JUMPER_MATERIALLOADER_HPP
