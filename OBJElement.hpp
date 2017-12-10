//
// Created by Jean-Luc Thumm on 12/10/17.
//

#ifndef JUMPER_OBJELEMENT_HPP
#define JUMPER_OBJELEMENT_HPP

#include "GLinclude.h"
#include "Material.h"
#include <memory>
#include <string>

class OBJElement {
public:
    OBJElement() : VAO{0} {}

    GLuint VAO;
    std::shared_ptr<const Material> material;
    std::string name;
};


#endif //JUMPER_OBJELEMENT_HPP
