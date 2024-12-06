#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <windows.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "application/application.h"
#include "glframework/camera/camera.h"
#include "glframework/shader/shader.h"
#include "glframework/texture/texture.h"

#ifdef NV
    extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
#endif

int scr_width = 800;
int scr_height = 600;

GLuint vbo, light_vao, object_vao;
Camera* camera = nullptr;
Shader* light_shader = nullptr, * object_shader = nullptr;
Texture* diffuse_map = nullptr, *specular_map = nullptr;

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

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

void LoadCamera() {
	camera = new Camera(glm::vec3(0.0, 0.0, 3.0));
}

void KeyInput() {
	if (glfwGetKey(app->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(app->getWindow(), GLFW_TRUE);

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

void LoadShader() {
    light_shader = new Shader("assets/shaders/light_shader/light_vertex.vert", "assets/shaders/light_shader/light_fragment.frag");
    object_shader = new Shader("assets/shaders/object_shader/object_vertex.vert", "assets/shaders/object_shader/object_fragment.frag");
}

void LoadTexture() {
    diffuse_map = new Texture("assets/textures/diffuse_map.png", 0);
    specular_map = new Texture("assets/textures/specular_map.png", 1);
}

void render() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    KeyInput();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    object_shader->begin();
    object_shader->setVec3("viewPos", camera->getPosition());
    //material
    object_shader->setInt("material.diffuse", 0);
    object_shader->setInt("material.specular", 1);
    object_shader->setFloat("material.shininess", 32.0f);
    // directional light
    object_shader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    object_shader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    object_shader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    object_shader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    object_shader->setVec3("pointLights[0].position", pointLightPositions[0]);
    object_shader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    object_shader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    object_shader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    object_shader->setFloat("pointLights[0].constant", 1.0f);
    object_shader->setFloat("pointLights[0].linear", 0.09f);
    object_shader->setFloat("pointLights[0].quadratic", 0.032f);
    // point light 2
    object_shader->setVec3("pointLights[1].position", pointLightPositions[1]);
    object_shader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    object_shader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    object_shader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    object_shader->setFloat("pointLights[1].constant", 1.0f);
    object_shader->setFloat("pointLights[1].linear", 0.09f);
    object_shader->setFloat("pointLights[1].quadratic", 0.032f);
    // point light 3
    object_shader->setVec3("pointLights[2].position", pointLightPositions[2]);
    object_shader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    object_shader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    object_shader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    object_shader->setFloat("pointLights[2].constant", 1.0f);
    object_shader->setFloat("pointLights[2].linear", 0.09f);
    object_shader->setFloat("pointLights[2].quadratic", 0.032f);
    // point light 4
    object_shader->setVec3("pointLights[3].position", pointLightPositions[3]);
    object_shader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    object_shader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    object_shader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    object_shader->setFloat("pointLights[3].constant", 1.0f);
    object_shader->setFloat("pointLights[3].linear", 0.09f);
    object_shader->setFloat("pointLights[3].quadratic", 0.032f);
    // spotLight
    object_shader->setVec3("spotLight.position", camera->getPosition());
    object_shader->setVec3("spotLight.direction", camera->getFront());
    object_shader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    object_shader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    object_shader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    object_shader->setFloat("spotLight.constant", 1.0f);
    object_shader->setFloat("spotLight.linear", 0.09f);
    object_shader->setFloat("spotLight.quadratic", 0.032f);
    object_shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    object_shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
    //m, v, p
    glm::mat4 projection = glm::perspective(glm::radians(camera->getFov()), (float)scr_width / (float)scr_height, 0.1f, 100.0f);
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 model = glm::mat4(1.0);
    object_shader->setMat4("projection", projection);
    object_shader->setMat4("view", view);
    object_shader->setMat4("model", model);
    diffuse_map->begin();
    specular_map->begin();
    glBindVertexArray(object_vao);
    for (unsigned int i = 0; i < 10; ++i) {
        model = glm::mat4(1.0);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        object_shader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);
    object_shader->end();


    light_shader->begin();
    light_shader->setMat4("projection", projection);
    light_shader->setMat4("view", view);
    glBindVertexArray(light_vao);
    for (unsigned int i = 0; i < 4; i++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f));
        light_shader->setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);
    light_shader->end();

}

int main() {
	app->init(scr_width, scr_height);
	app->setmousecallback(MouseCallback);
	app->setscrollcallback(ScrollCallback);

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