#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "application/application.h"
#include "glframework/shader/shader.h"
#include "glframework/camera/camera.h"

int scr_width = 800;
int scr_height = 600;

GLuint vbo, cube_vao, lightcube_vao;
Shader* lighting_shader = nullptr, *lightcube_shader = nullptr;
Camera* camera = nullptr;

float lastX = scr_width / 2.0f;
float lastY = scr_height / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
};

void LoadCamera() {
    camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
}

void processInput() {
    if (glfwGetKey(app->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(app->getWindow(), true);

    if (glfwGetKey(app->getWindow(), GLFW_KEY_W) == GLFW_PRESS) camera->KeyMove("FORWARD", deltaTime);
    if (glfwGetKey(app->getWindow(), GLFW_KEY_A) == GLFW_PRESS) camera->KeyMove("LEFT", deltaTime);
    if (glfwGetKey(app->getWindow(), GLFW_KEY_S) == GLFW_PRESS) camera->KeyMove("BACKWARD", deltaTime);
    if (glfwGetKey(app->getWindow(), GLFW_KEY_D) == GLFW_PRESS) camera->KeyMove("RIGHT", deltaTime);
}

void mouse_callback(double xposIn, double yposIn) {
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

    camera->MouseMove(xoffset, yoffset);
}

void scroll_callback(double xoffset, double yoffset) {
    camera->MouseSroll(yoffset);
}

void Loadvertex() {
    glGenVertexArrays(1, &cube_vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(cube_vao);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
   
    glGenVertexArrays(1, &lightcube_vao);
    glBindVertexArray(lightcube_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Loadshader() {
    lighting_shader = new Shader("assets/shaders/color_vertex.vert", "assets/shaders/color_fragment.frag");
    lightcube_shader = new Shader("assets/shaders/lightcube_vertex.vert", "assets/shaders/lightcube_fragment.frag");
}

void render() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    lighting_shader->begin();
    lighting_shader->setVec3("cubecolor", glm::vec3(1.0f, 0.5f, 0.31f));
    lighting_shader->setVec3("lightcolor", glm::vec3(1.0f, 1.0f, 1.0f));

    glm::mat4 projection = glm::perspective(glm::radians(camera->getZoom()), (float)scr_width / (float)scr_height, 0.1f, 100.0f);
    glm::mat4 view = camera->getViewMatrix();
    lighting_shader->setMat4("projection", projection);
    lighting_shader->setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    lighting_shader->setMat4("model", model);

    glBindVertexArray(cube_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
 
    glBindVertexArray(0);
    lighting_shader->end();
    
    lightcube_shader->begin();
    lightcube_shader->setMat4("projection", projection);
    lightcube_shader->setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    lightcube_shader->setMat4("model", model);

    glBindVertexArray(lightcube_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
   
    glBindVertexArray(0);
    lightcube_shader->end();
    
}

int main() {
    app->init(scr_width, scr_height);
    app->setMouseCallback(mouse_callback);
    app->setScrollCallback(scroll_callback);

    glViewport(0, 0, scr_width, scr_width);

    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    glEnable(GL_DEPTH_TEST);

    LoadCamera();
    Loadshader();
    Loadvertex();

    while (app->update()) {
        render();
    }

    app->destory();

    delete lighting_shader;
    delete lightcube_shader;
    delete camera;

	return 0;
}