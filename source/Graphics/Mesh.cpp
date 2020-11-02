#include "Mesh.hpp"

#include <GL/glew.h>

Mesh::Mesh(const std::string& name,
    std::vector<Vertex>&& vertices, 
    std::vector<uint32_t>&& indices) noexcept
    : name(name)
    , vertices(std::move(vertices))
    , indices(std::move(indices)) {
    SetupMesh();
}

void Mesh::Cleanup() {
    glDeleteVertexArrays(1, &VertexArrayObject);

    glDeleteBuffers(1, &VertexBufferObject);
    glDeleteBuffers(1, &ElementBufferObject);
}

void Mesh::SetupMesh() {
    glGenVertexArrays(1, &VertexArrayObject);

    glGenBuffers(1, &VertexBufferObject);
    glGenBuffers(1, &ElementBufferObject);

    glBindVertexArray(VertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));

    glBindVertexArray(0);
}

void Mesh::Draw(Shader const& shader) const {
    //glEnable(GL_DEPTH_TEST);
    glBindVertexArray(VertexArrayObject);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}