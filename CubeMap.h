#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "GLinclude.h"
#include "Shader.h"
#include <vector>
#include <string>
#include <memory>

class CubeMap {
public:
    static GLuint genCubeMapTexture(const std::vector<std::string> &names, std::string directory);

    CubeMap(const std::vector<std::string> &names, std::string directory, std::shared_ptr<Shader> shader);

    void draw(const glm::mat4 &view, const glm::mat4 &projection);

    GLuint TextureID();

private:
    static float vertices[];
    GLuint texID;
    GLuint VAO, VBO;
    std::shared_ptr<Shader> shader;
};

#endif
