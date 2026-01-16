#include "EngineCompass.h"

#include "UserInterface.h"
#include "UI_Engine.h"
#include "UI_ProjectsPanel.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


struct axisVertex {
    glm::vec3 pos;
    glm::vec3 color;
};

axisVertex compassVertices[] = {
    // X axis (red)
    {{0, 0, 0}, {1, 0, 0}},
    {{1, 0, 0}, {1, 0, 0}},

    // Y axis (green)
    {{0, 0, 0}, {0, 1, 0}},
    {{0, 1, 0}, {0, 1, 0}},

    // Z axis (blue)
    {{0, 0, 0}, {0, 0, 1}},
    {{0, 0, 1}, {0, 0, 1}},
};



GLuint compassIndices[] = {
    3, 0,
    3, 1,
    3, 2
};



void Compass::Render(glm::mat4 cameraView)
{
    // strips translation
    glm::mat4 view = glm::mat4(glm::mat3(cameraView));
    glm::mat4 projection = glm::ortho(-2.f, 2.f, -2.f, 2.f, -10.f, 10.f);
    glLineWidth(lineWidth);

    glDisable(GL_DEPTH_TEST);

    // compass transform
    //        ( x ,  y) (size_x), (size_y)
    glViewport(225, 1150, size, size);

    defaultShader.Activate();

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(0.8f));
    defaultShader.setMat4("model", model);
    defaultShader.setMat4("view", view);
    defaultShader.setMat4("projection", projection);

    vao.Bind();
    //glDisable(GL_CULL_FACE);
    //glLineWidth(3.0f);
    glDrawArrays(GL_LINES, 0, 6);
    vao.Unbind();

    glViewport(0, 0, VoxlEngine::windowWidth, VoxlEngine::windowHeight);
    glEnable(GL_DEPTH_TEST);
}

Compass::Compass(Shader& inputShader) : defaultShader(inputShader)
{
    std::cout << "Building out Compass mesh..." << std::endl;

    //VAO initialization 
    vao.Bind();

    //VBO initialization
    vbo = new VBO(compassVertices, sizeof(compassVertices));


    GLsizei stride = sizeof(axisVertex);

    //OpenGL stuff
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);                  // pos
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float))); // color
    glEnableVertexAttribArray(1);

    vao.Unbind();

    std::cout << "Done initializing compass mesh." << std::endl;
}

//DEconstructor
Compass::~Compass()
{
    delete vbo;
}


