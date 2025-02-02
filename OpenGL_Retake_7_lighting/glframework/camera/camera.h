#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

class Camera {
public:
	Camera(glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f),
			const float &yaw = YAW, const float &pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	~Camera();

	

	void KeyMove(std::string direction, float deltaTime);

	void MouseMove(float xoffset, float yoffset, GLboolean constrianPitch = true);

	void MouseScroll(float yoffset);

	glm::mat4 getViewMatrix();
	
	glm::vec3 getPosition();

	float getFov();

private:

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	
	float Yaw;
	float Pitch;

	float moveSpeed;
	float moveSensitivity;
	float fov;

	void updateCameraVector();

};