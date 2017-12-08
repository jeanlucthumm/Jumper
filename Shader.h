//
// Created by Jean-Luc Thumm on 11/12/17.
//

#ifndef PA2_SHADER_H
#define PA2_SHADER_H


#include "GLinclude.h"
#include <string>
#include <glm/vec3.hpp>
#include <glm/detail/type_mat.hpp>

class Shader {
public:
    Shader(std::string vertPath, std::string fragPath);
    ~Shader();
    Shader(const Shader &) = delete;
    Shader &operator=(Shader &) = delete;
    Shader(Shader &&other) noexcept;
    Shader &operator=(Shader &&other) noexcept;

    void use();

    void put(const char *tag, float value);
    void put(const char *tag, int value);
    void put(const char *tag, const glm::vec3 &value);
    void put(const char *tag, const glm::vec4 &value);
    void put(const char *tag, const glm::mat4 &value);

private:
    GLuint id;
    std::string vertPath;
    std::string fragPath;

    GLuint compile(std::string vertex_file_path, std::string fragment_file_path);
};


#endif //PA2_SHADER_H
