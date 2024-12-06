#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "application/application.h"
#include "glframework/shader/shader.h"
#include "glframework/texture/texture.h"
#include "glframework/camera/camera.h"

int scr_width = 800;
int scr_height = 600;

GLuint vbo, object_vao, light_vao;
Shader* light_shader = nullptr, *object_shader = nullptr;
Camera* camera = nullptr;

float lastX = scr_width / 2.0f;
float lastY = scr_height / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

void LoadVertex() {
    glGenBuffers(1, &vbo);


    glGenVertexArrays(1, &object_vao); 
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(object_vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glDisableVertexAttribArray(1);



    glGenVertexArrays(1, &light_vao);
    glBindVertexArray(light_vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void LoadCamera() {
	camera = new Camera(glm::vec3(0.0, 0.0, 3.0));
}

void processInput() {
    if (glfwGetKey(app->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(app->getWindow(), true);

    if (glfwGetKey(app->getWindow(), GLFW_KEY_W) == GLFW_PRESS) camera->KeyMove("FORWARD", deltaTime);
    if (glfwGetKey(app->getWindow(), GLFW_KEY_S) == GLFW_PRESS) camera->KeyMove("BACKWARD", deltaTime);
    if (glfwGetKey(app->getWindow(), GLFW_KEY_A) == GLFW_PRESS) camera->KeyMove("LEFT", deltaTime);
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
    camera->MouseScroll(yoffset);
}

void LoadShader() {
	light_shader = new Shader("assets/shaders/light_shader/vertex.vert", "assets/shaders/light_shader/fragment.frag");
    object_shader = new Shader("assets/shaders/object_shader/vertex.vert", "assets/shaders/object_shader/fragment.frag");
}

void render() {

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    processInput();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    object_shader->begin();
    object_shader->setVec3("cubeColor", 1.0f, 0.5f, 0.31f);
    object_shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    object_shader->setVec3("lightPos", lightPos);
    object_shader->setVec3("viewPos", camera->getPosition());
    glm::mat4 projection = glm::perspective(glm::radians(camera->getFov()), (float)scr_width / (float)scr_height, 0.1f, 100.0f);
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    object_shader->setMat4("projection", projection);
    object_shader->setMat4("view", view);
    object_shader->setMat4("model", model);
    glBindVertexArray(object_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    object_shader->end();



    light_shader->begin();
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
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
    app->setmouseCallback(mouse_callback);
    app->setscrollCallback(scroll_callback);

	glViewport(0, 0, scr_width, scr_height);
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glEnable(GL_DEPTH_TEST);

    LoadCamera();
    LoadVertex();
    LoadShader();

	while (app->update()) {
		render();
	}

	app->destory();

	delete light_shader;
	delete object_shader;
	delete camera;

	return 0;
}