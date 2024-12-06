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

float quadVertices[] = {
    // Î»ÖÃ          // ÑÕÉ«
    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
     0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
    -0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

    -0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
     0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
     0.05f,  0.05f,  0.0f, 1.0f, 1.0f
};

glm::vec2 translations[100];

GLuint instanceVBO, VBO, VAO;

void InstanceArray() {
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(glm::vec2), &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);
}

int main() {
    App->init(scr_width, scr_height);
    App->setMouseCallback(MouseCallback);
    App->setScrollCallback(ScrollCallback);

    glViewport(0, 0, scr_width, scr_height);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_DEPTH_TEST);

    Shader shader("assets/shaders/100/vertex.vs", "assets/shaders/100/fragment.fs");

    int index = 0;
    float offset = 0.1f;
    for (int y = -10; y < 10; y += 2) {
        for (int x = -10; x < 10; x += 2) {
            glm::vec2 translation;
            translation.x = (float)x / 10.0f + offset;
            translation.y = (float)y / 10.0f + offset;
            translations[index++] = translation;
        }
    }

    InstanceArray();

    while (App->update()) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.begin();
        for (unsigned int i = 0; i < 100; ++i) {
            std::stringstream ss;
            std::string index;
            ss << i;
            index = ss.str();
            shader.setVec2(("offsets[" + index + "]").c_str(), translations[i]);
        }
        glBindVertexArray(VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
        shader.end();
    }

    App->destory();

    return 0;
}