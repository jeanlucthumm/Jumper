#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "GLinclude.h"
#include "Node.h"
#include "Material.h"
#include "Shader.h"
#include <vector>
#include <memory>

class Geometry : public Node {
public:
    Geometry(std::string objPath, std::shared_ptr<Shader> shader);

    void draw(const glm::mat4 &parent,
              const glm::mat4 &view,
              const glm::mat4 &projection) override;

    void update() override;

    std::list<Node *> hit(const Ray &ray) override;

private:
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;

    GLuint VAO, EBO, VVBO, NVBO;

    glm::mat4 base;
    std::shared_ptr<Shader> shader;

    bool parse(const std::string &path);
};

#endif // GEOMETRY_H