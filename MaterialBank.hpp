//
// Created by Jean-Luc Thumm on 12/10/17.
//

#ifndef JUMPER_MATERIALLOADER_HPP
#define JUMPER_MATERIALLOADER_HPP

#include "Material.h"
#include <string>
#include <memory>
#include <unordered_map>

class MaterialBank {
public:
    static const MaterialBank *I();

    MaterialBank();

    bool parse(const std::string &path);
    const std::shared_ptr<const Material> &get(const std::string &name) const;
    bool store(Material material);

private:
    static MaterialBank *instance;

    GLuint putTexture(const std::string &path);

    std::unordered_map<std::string, std::shared_ptr<const Material>> table;
};


#endif //JUMPER_MATERIALLOADER_HPP
