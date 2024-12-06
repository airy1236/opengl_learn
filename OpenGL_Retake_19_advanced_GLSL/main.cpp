#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "application/application.h"
#include "glframework/camera/camera.h"
#include "glframework/shader/shader.h"

int scr_width = 800;
int scr_height = 600;

float lastX = scr_width / 2.0f;
float lastY = scr_height / 2.0f;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0, 0.0, 3.0));

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
	camera.ScrollMove(yoffset);
}

float cubeVertices[] = {
    // positions         
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

GLuint VAO, VBO;

void LoadVertex() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

GLuint UBO;

int main() {
	App->init(scr_width, scr_height);
	App->setMouseCallback(MouseCallback);
	App->setScrollCallback(ScrollCallback);

    glViewport(0, 0, scr_width, scr_height);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_DEPTH_TEST);

    LoadVertex();

    Shader redShader("assets/shaders/red/vertex.vert", "assets/shaders/red/fragment.frag"), 
       blueShader("assets/shaders/blue/vertex.vert", "assets/shaders/blue/fragment.frag"), 
       greenShader("assets/shaders/green/vertex.vert", "assets/shaders/green/fragment.frag"), 
       yellowShader("assets/shaders/yellow/vertex.vert", "assets/shaders/yellow/fragment.frag");

    //uniform缓冲
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    //uniform块索引
    GLuint red_index = glGetUniformBlockIndex(redShader.getProgram(), "Matrices");
    GLuint blue_index = glGetUniformBlockIndex(blueShader.getProgram(), "Matrices");
    GLuint green_index = glGetUniformBlockIndex(greenShader.getProgram(), "Matrices");
    GLuint yellow_index = glGetUniformBlockIndex(yellowShader.getProgram(), "Matrices");
    glUniformBlockBinding(redShader.getProgram(), red_index, 0);
    glUniformBlockBinding(blueShader.getProgram(), blue_index, 0);
    glUniformBlockBinding(greenShader.getProgram(), green_index, 0);
    glUniformBlockBinding(yellowShader.getProgram(), yellow_index, 0);
    //绑定uniform缓冲到相同的绑定点上
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, 2 * sizeof(glm::mat4));
    //向uniform缓冲中添加数据
    glm::mat4 projection = glm::perspective(45.0f, (float)scr_width / (float)scr_height, 0.1f, 100.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

	while (App->update()) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        
        glBindVertexArray(VAO);

        redShader.begin();
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f));
        redShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        redShader.end();

        blueShader.begin();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f));
        blueShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        blueShader.end();

        greenShader.begin();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f));
        greenShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        greenShader.end();

        yellowShader.begin();
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f));
        yellowShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        yellowShader.end();

        glBindVertexArray(0);
	}

	App->destory();

	return 0;
}