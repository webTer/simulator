#include "Skybox.hpp"
#include "../STBI/stb_image.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>


std::shared_ptr<Skybox> Skybox::Create(const std::vector<std::filesystem::path>& faces) {
    auto instance = std::make_shared<Skybox>();

    // todo split
    //1 . load textures
    glGenTextures(1, &instance->id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, instance->id);

    int32_t w{}, h{}, channels{};

    uint32_t offset{ 0u };
    for (const auto& facePath : faces) {
        auto* data = stbi_load(facePath.string().c_str(), &w, &h, &channels, 0);

        if (data != nullptr) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + offset, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else {
            throw std::runtime_error("Failed to load face.");
        }

        stbi_image_free(data);
        ++offset;
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    //2. gen buffers

    float vertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &instance->vao);

    glGenBuffers(1, &instance->vbo);

    glBindVertexArray(instance->vao);
    glBindBuffer(GL_ARRAY_BUFFER, instance->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));

    glBindVertexArray(0);

    return instance;
}

void Skybox::Cleanup() {
    glDeleteTextures(1, &id);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void Skybox::Bind() const  {
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void Skybox::Draw(const std::shared_ptr<Shader>& shader) const {
    glDepthMask(GL_FALSE);
    
    shader->Use();

    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glDepthMask(GL_TRUE);
}