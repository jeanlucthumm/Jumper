#include "Geometry.h"
#include "Window.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <glm/ext.hpp>

Geometry::Geometry(std::string objPath, std::shared_ptr<Shader> shader) : shader{std::move(shader)} {
    if (!parse(objPath)) return;

    // OpenGL

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VVBO);
    glGenBuffers(1, &NVBO);
    glBindVertexArray(VAO);

    // positions
    glBindBuffer(GL_ARRAY_BUFFER, VVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size()) * sizeof(glm::vec3),
                 &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid *) 0);

    // normals
    glBindBuffer(GL_ARRAY_BUFFER, NVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(normals.size()) * sizeof(glm::vec3),
                 &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid *) 0);

    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(indices.size()) * sizeof(unsigned int), &indices[0],
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Geometry::draw(const glm::mat4 &parent,
                    const glm::mat4 &view,
                    const glm::mat4 &projection) {
    shader->use();
    shader->put("model", parent);
    shader->put("view", view);
    shader->put("projection", projection);
    shader->put("cameraPos", Window::Instance().CameraPos());

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Window::Instance().SkyboxTexture());
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()),
                   GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Geometry::update() {

}

bool Geometry::parse(const std::string &path) {
    std::ifstream in{path};
    if (!in.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        return false;
    }

    std::string line, next;
    std::string id;
    float x, y, z;  // vertex and normals
    std::string f;

    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    minX = minY = minZ = std::numeric_limits<float>::max();
    maxX = maxY = maxZ = std::numeric_limits<float>::min();

    while (getline(in, line)) {
        std::stringstream ss{line};
        ss >> id;

        // look for normal
        if (id[0] == '#') continue;
        if (id == "vn") {
            ss >> x;
            ss >> y;
            ss >> z;
            glm::vec3 normal{x, y, z};

            // read vertex on next line
            if (!getline(in, next)) break;
            std::stringstream vs{next};
            vs >> id;
            if (id != "v") continue;
            vs >> x;
            vs >> y;
            vs >> z;
            glm::vec3 vertex{x, y, z};

            normal = glm::normalize(normal); // normalize colors

            // update extremes for centering
            if (x < minX) minX = x;
            else if (x > maxX) maxX = x;
            if (y < minY) minY = y;
            else if (y > maxY) maxY = y;
            if (z < minZ) minZ = z;
            else if (z > maxZ) maxZ = z;

            vertices.push_back(vertex);
            normals.push_back(normal);
        }

        // look for face indices
        if (id == "f") {
            for (int i = 0; i < 3; i++) {
                ss >> f;
                std::string indexStr = f.substr(0, f.find("//"));
                auto index = static_cast<unsigned int>(stoi(indexStr));
                indices.push_back(index - 1); // obj starts at 1, GL starts at 0
            }
        }

    }

    // center object
    float avgX = (maxX + minX) / 2;
    float avgY = (maxY + minY) / 2;
    float avgZ = (maxZ + minZ) / 2;
    glm::vec3 center = glm::vec3{avgX, avgY, avgZ};
    glm::mat4 T = glm::translate(-center);

    // scale object
    float rangeX = (maxX - minX) / 2;
    float rangeY = (maxY - minY) / 2;
    float rangeZ = (maxZ - minZ) / 2;
    float range = std::max({rangeX, rangeY, rangeZ});
    glm::mat4 S = glm::scale((1.0f / range) * glm::vec3{1.0f});

    base = S * T;

    std::cout << "Done parsing: " << path << ". ";
    std::cout << "Vertices: " << vertices.size() << ". ";
    std::cout << "Normals: " << normals.size() << ". ";
    std::cout << "Indices: " << indices.size() << std::endl;
    in.close();
    return true;
}

std::list<Node *> Geometry::hit(const Ray &ray) {
    return std::list<Node *>{};
}

