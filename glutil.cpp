#include "glutil.hpp"

GLuint glutil::makeStdVAO(const std::vector<glm::vec3> &vertices) {
    GLuint VAO, vertexVBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertexVBO);

    auto vsize = static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec3));

    glBindVertexArray(VAO);

    // positions
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, vsize, &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    return VAO;
}

GLuint glutil::makeStdVAO(const std::vector<glm::vec3> &vertices,
                          const std::vector<glm::vec3> &normals) {
    GLuint VAO, vertexVBO, normalVBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertexVBO);
    glGenBuffers(1, &normalVBO);

    auto vsize = static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec3));
    auto nsize = static_cast<GLsizeiptr>(normals.size() * sizeof(glm::vec3));

    glBindVertexArray(VAO);

    // positions
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, vsize, &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    // normals
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, nsize, &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    return VAO;
}

GLuint glutil::makeStdVAO(const std::vector<glm::vec3> &vertices,
                          const std::vector<glm::vec3> &normals,
                          const std::vector<glm::vec2> &uvs) {
    GLuint VAO, vertexVBO, normalVBO, uvVBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertexVBO);
    glGenBuffers(1, &normalVBO);
    glGenBuffers(1, &uvVBO);

    auto vsize = static_cast<GLsizeiptr>(vertices.size() * sizeof(glm::vec3));
    auto nsize = static_cast<GLsizeiptr>(normals.size() * sizeof(glm::vec3));
    auto usize = static_cast<GLsizeiptr>(uvs.size() * sizeof(glm::vec2));

    glBindVertexArray(VAO);

    // positions
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, vsize, &vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    // normals
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, nsize, &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    // uvs
    glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
    glBufferData(GL_ARRAY_BUFFER, usize, &uvs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);

    return VAO;
}
