#pragma once

#include "GraphicalResource.hpp"
#include "Mesh.hpp"

#include <assimp/scene.h>

class ResourceManager;

class Model final : public GraphicalResource {
    std::vector<Mesh> meshes;
    std::string name;
public:

    explicit Model(const std::string& name);
    virtual ~Model() = default;

    void Draw(const Shader& shader) const;
    void Cleanup() override;
private:
    void InitalizeFromScene(const aiScene* scene);
    void ProcessNode(const aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(const aiMesh* mesh, const aiScene* scene);

    friend ResourceManager;

};