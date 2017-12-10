//
// Created by Jean-Luc Thumm on 11/12/17.
//

#include "GLinclude.h"
#include <ios>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <glm/mat4x4.hpp>
#include "Shader.h"

using std::cout;
using std::endl;
using std::cerr;


Shader::Shader(std::string vertPath, std::string fragPath)
        : vertPath{std::move(vertPath)}, fragPath{std::move(fragPath)} {
    id = compile(vertPath, fragPath);
}

GLuint Shader::compile(std::string vertex_file_path, std::string fragment_file_path) {
    // Read code from file
    std::ifstream vertStream{vertPath};
    std::ifstream fragStream{fragPath};

    if (!vertStream.is_open()) {
        cerr << "Could not open shader file: " << vertPath << endl;
        return 0;
    }
    if (!fragStream.is_open()) {
        cerr << "Could not open shader file: " << fragPath << endl;
        return 0;
    }

    std::string vertCode{std::istreambuf_iterator<char>{vertStream},
                         std::istreambuf_iterator<char>{}};
    std::string fragCode{std::istreambuf_iterator<char>{fragStream},
                         std::istreambuf_iterator<char>{}};

    vertStream.close();
    fragStream.close();

    // Compile code
    GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);
    GLint result = GL_FALSE;
    GLint infoLogSize;

    cout << "Compiling shader: " << vertPath << " ... ";
    const char *vertCodePt = vertCode.c_str();
    glShaderSource(vertID, 1, &vertCodePt, nullptr);
    glCompileShader(vertID);

    glGetShaderiv(vertID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertID, GL_INFO_LOG_LENGTH, &infoLogSize);

    if (infoLogSize > 0) {
        std::vector<char> error(static_cast<unsigned long>(infoLogSize + 1));
        glGetShaderInfoLog(vertID, infoLogSize, nullptr, &error[0]);
        printf("%s\n", &error[0]);
    }
    else {
        cout << "Success!" << endl;
    }

    cout << "Compiling shader: " << fragPath << " ... ";
    const char *fragCodePt = fragCode.c_str();
    glShaderSource(fragID, 1, &fragCodePt, nullptr);
    glCompileShader(fragID);

    glGetShaderiv(fragID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragID, GL_INFO_LOG_LENGTH, &infoLogSize);

    if (infoLogSize > 0) {
        std::vector<char> error(static_cast<unsigned long>(infoLogSize + 1));
        glGetShaderInfoLog(fragID, infoLogSize, nullptr, &error[0]);
        printf("%s\n", &error[0]);
    }
    else {
        cout << "Success!" << endl;
    }

    // Link the program
    cout << "Linking program" << endl;
    GLuint program = glCreateProgram();
    glAttachShader(program, vertID);
    glAttachShader(program, fragID);
    glLinkProgram(program);

    // Check the program
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogSize);
    if (infoLogSize > 0) {
        std::vector<char> error(static_cast<unsigned long>(infoLogSize + 1));
        glGetProgramInfoLog(program, infoLogSize, nullptr, &error[0]);
        printf("%s\n", &error[0]);
    }
    glDetachShader(program, vertID);
    glDetachShader(program, fragID);

    glDeleteShader(vertID);
    glDeleteShader(fragID);

    return program;
}

void Shader::put(const std::string &tag, float value) {
    glUniform1f(glGetUniformLocation(id, tag.c_str()), value);
}

void Shader::put(const std::string &tag, int value) {
    glUniform1i(glGetUniformLocation(id, tag.c_str()), value);
}

void Shader::put(const std::string &tag, const glm::vec3 &value) {
    glUniform3f(glGetUniformLocation(id, tag.c_str()), value.x, value.y, value.z);
}

void Shader::put(const std::string &tag, const glm::vec4 &value) {
    glUniform4f(glGetUniformLocation(id, tag.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::put(const std::string &tag, const glm::mat4 &value) {
    glUniformMatrix4fv(glGetUniformLocation(id, tag.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::use() {
    glUseProgram(id);
}

Shader::~Shader() {
    glDeleteProgram(id);
}

Shader::Shader(Shader &&other) noexcept {
    id = other.id;
    fragPath = other.fragPath;
    vertPath = other.vertPath;
    other.id = 0;
}

Shader &Shader::operator=(Shader && other) noexcept {
    id = other.id;
    fragPath = other.fragPath;
    vertPath = other.vertPath;
    other.id = 0;
    return *this;
}
