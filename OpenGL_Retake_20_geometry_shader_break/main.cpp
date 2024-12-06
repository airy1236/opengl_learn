#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "application/application.h"
#include "glframework/shader/shader.h"
#include "glframework/camera/camera.h"
#include "glframework/mesh/mesh.h"
#include "glframework/model/model.h"
#include "glframework/texture/texture.h"

int scr_width = 1920;
int scr_height = 1080;

Camera camera(glm::vec3(0.0, 0.0, 3.0));

float lastX = scr_width / 2.0f;
float lastY = scr_height / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput() {
    if (glfwGetKey(App->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(App->getWindow(), true);

    if (glfwGetKey(App->getWindow(), GLFW_KEY_W) == GLFW_PRESS) camera.KeyMove(1, deltaTime);
    if (glfwGetKey(App->getWindow(), GLFW_KEY_A) == GLFW_PRESS) camera.KeyMove(2, deltaTime);
    if (glfwGetKey(App->getWindow(), GLFW_KEY_S) == GLFW_PRESS) camera.KeyMove(3, deltaTime);
    if (glfwGetKey(App->getWindow(), GLFW_KEY_D) == GLFW_PRESS) camera.KeyMove(4, deltaTime);
}

void MouseCallback(double xposIn, double yposIn) {
    float xpos = xposIn;
    float ypos = yposIn;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.MouseMove(xoffset, yoffset);
}

void ScrollCallback(double xoffset, double yoffset) {
    camera.MouseScroll(yoffset);
}

int main() {
    App->init(scr_width, scr_height);
    App->setMouseCallback(MouseCallback);
    App->setScrollCallback(ScrollCallback);

    glViewport(0, 0, scr_width, scr_height);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_DEPTH_TEST);

    Shader breakshader("assets/shaders/break/vertex.vs", "assets/shaders/break/fragment.fs", "assets/shaders/break/geometry.geom"),
           normalshader("assets/shaders/normal_visualization/normal.vs", "assets/shaders/normal_visualization/normal.fs", "assets/shaders/normal_visualization/normal.geom"),
           shader("assets/shaders/normal_visualization/vertex.vs", "assets/shaders/normal_visualization/fragment.fs");

    Model m_model("assets/models/nanosuit.obj");

    while (App->update()) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)scr_width / (float)scr_height, 1.0f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();;
        glm::mat4 model = glm::mat4(1.0f);
        shader.begin();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setMat4("model", model);
        m_model.Draw(shader);
        shader.end();

        normalshader.begin();
        normalshader.setMat4("projection", projection);
        normalshader.setMat4("view", view);
        normalshader.setMat4("model", model);
        m_model.Draw(normalshader);
        normalshader.end();
    }

    App->destory();

    return 0;
}