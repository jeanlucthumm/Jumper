#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "GLinclude.h"
#include "Node.h"
#include "Material.h"
#include "Shader.h"
#include "OBJBank.h"
#include <vector>
#include <memory>

class Geometry : public Node {
public:
    Geometry(OBJBank::refID dataID, std::shared_ptr<Shader> shader);

    void draw(const glm::mat4 &parent,
              const glm::mat4 &view,
              const glm::mat4 &projection) override;

    void update() override;

    std::list<Node *> hit(const Ray &ray) override;

private:
    GLuint VAO, EBO, VVBO, NVBO;
    OBJBank::refID dataID;
    const OBJBank::Data &data;

    glm::mat4 base;
    std::shared_ptr<Shader> shader;
};

#endif // GEOMETRY_H