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
    Geometry(MeshBank::refID dataID, std::shared_ptr<Shader> shader);

    void draw(const glm::mat4 &parent,
              const glm::mat4 &view,
              const glm::mat4 &projection) override;

    void update(std::chrono::milliseconds delta) override;

    std::list<Node *> hit(const Ray &ray) override;

private:
    void put(const std::shared_ptr<const Material> &material);

    GLuint VAO, EBO, VVBO, NVBO;
    MeshBank::refID dataID;
    const std::vector<OBJElement> &data;

    glm::mat4 base;
    std::shared_ptr<Shader> shader;
};

#endif // GEOMETRY_H