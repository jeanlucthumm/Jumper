//
// Created by Jean-Luc Thumm on 12/9/17.
//
#include "Material.h"


void Material::putYourself(std::shared_ptr<Shader> &shader) {
    shader->use();
    shader->put("material.ambient", ka);
    shader->put("material.diffuse", kd);
    shader->put("material.specular", ks);
    shader->put("material.shiny", shiny);
}
