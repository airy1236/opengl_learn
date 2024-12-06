#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "application/application.h"
#include "glframework/camera/camera.h"
#include "glframework/shader/shader.h"
#include "glframework/texture/texture.h"

int scr_width = 800;
int scr_height = 600;

GLuint vbo, light_vao, object_vao;
Camera* camera = nullptr;
Shader* light_shader = nullptr, *object_shader = nullptr;
Texture* diffuse_map = nullptr, *specular_map = nullptr;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

float lastX = scr_width / 2.0f;
float lastY = scr_height / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

void LoadCamera() {
    camera = new Camera(glm::vec3(0.0, 0.0, 3.0));
}

void KeyInput() {
    if (glfwGetKey(app->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(app->getWindow(), GL_TRUE);

    if (glfwGetKey(app->getWindow(), GLFW_KEY_W) == GLFW_PRESS) camera->KeyMove("FORWARD", deltaTime);
    if (glfwGetKey(app->getWindow(), GLFW_KEY_S) == GLFW_PRESS) camera->KeyMove("BACKWARD", deltaTime);
    if (glfwGetKey(app->getWindow(), GLFW_KEY_A) == GLFW_PRESS) camera->KeyMove("LEFT", deltaTime);
    if (glfwGetKey(app->getWindow(), GLFW_KEY_D) == GLFW_PRESS) camera->KeyMove("RIGHT", deltaTime);
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

    camera->MouseMove(xoffset, yoffset);
}

void ScrollCallback(double xoffset, double yoffset) {
    camera->MouseScroll(yoffset);
}

void LoadShader() {
    light_shader = new Shader("assets/shaders/light_shader/light_vertex.vert", "assets/shaders/light_shader/light_fragment.frag");
    object_shader = new Shader("assets/shaders/object_shader/object_vertex.vert", "assets/shaders/object_shader/object_fragment.frag");
}

void LoadTexture() {
    diffuse_map = new Texture("assets/textures/lighting_map/diffuse_map.png", 0);
    specular_map = new Texture("assets/textures/lighting_map/specular_map.png", 1);
}

void LoadVertex() {
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &object_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(object_vao);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);



    glGenVertexArrays(1, &light_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glBindVertexArray(light_vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void render() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    KeyInput();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    object_shader->begin();
    object_shader->setVec3("viewPos", camera->getPos());
    //light
    object_shader->setVec3("light.position", lightPos);
    object_shader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    object_shader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    object_shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    //material
    object_shader->setInt("material.diffuse", 0);
    object_shader->setInt("material.specular", 1);
    object_shader->setFloat("material.shininess", 64.0f);
    //m,v,p
    glm::mat4 projection = glm::perspective(glm::radians(camera->getFov()), (float)scr_width / (float)scr_height, 0.1f, 100.0f);
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 model = glm::mat4(1.0);
    object_shader->setMat4("projection", projection);
    object_shader->setMat4("view", view);
    object_shader->setMat4("model", model);
    diffuse_map->begin();
    specular_map->begin();
    glBindVertexArray(object_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    object_shader->end();



    light_shader->begin();
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2)); 
    light_shader->setMat4("projection", projection);
    light_shader->setMat4("view", view);
    light_shader->setMat4("model", model);
    glBindVertexArray(light_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    light_shader->end();

}

int main() {
    app->init(scr_width, scr_height);
    app->setMouseCallback(MouseCallback);
    app->setScrollCallback(ScrollCallback);

    glViewport(0, 0, scr_width, scr_height);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_DEPTH_TEST);

    LoadCamera();
    LoadVertex();
    LoadShader();
    LoadTexture();

    while (app->update()) {
        render();
    }

    app->destory();

    delete camera;
    delete light_shader;
    delete object_shader;
    delete diffuse_map;
    delete specular_map;

	return 0;
}