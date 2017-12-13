#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "GLinclude.h"
#include "Node.h"
#include "Material.h"
#include "Shader.h"
#include "MeshBank.h"
#include <vector>
#include <memory>

class Geometry : public Node {
public:
    Geometry(MeshBank::refID dataID, std::shared_ptr<Shader> shader,
                 std::shared_ptr<Material> material);

    void draw(const glm::mat4 &parent,
              const glm::mat4 &view,
              const glm::mat4 &projection) override;

    void update() override;

    std::list<Node *> hit(const Ray &ray) override;

private:
    GLuint VAO, EBO, VVBO, NVBO;
    MeshBank::refID dataID;
    const std::vector<OBJElement> &data;

    glm::mat4 base;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Material> material;
};

#endif // GEOMETRY_H