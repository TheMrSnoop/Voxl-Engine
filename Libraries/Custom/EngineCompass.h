#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

#include "VoxlEngine.h"

#include "texture.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"


class Compass
{
public:
    struct directions
    {
        const char* label;
        glm::vec2 vec;
    };

    const int size = 150;
    const float lineWidth = 2.0f;

    Compass(Shader& inputShader);
    ~Compass();

    void Render(glm::mat4 cameraView);
private:
    //Rendering
    Shader& defaultShader;

    VAO vao;
    VBO* vbo;
};

