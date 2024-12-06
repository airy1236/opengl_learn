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

	Camera(glm::vec3 &position = glm::vec3(0.0, 0.0, 0.0), glm::vec3 &up = glm::vec3(0.0, 1.0, 0.0), 
			const float &pitch = PITCH, const float &yaw = YAW);

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float Pitch, float Yaw);

	~Camera();

	glm::mat4 getViewMatrix();

	glm::vec3 getPostion();

	glm::vec3 getFront();

	glm::vec3 getWorldUp();

	glm::vec3 getUp();

	glm::vec3 getRight();

	float getFov();

	void KeyMove(int dir, double deltaTime);

	void MouseMove(double xposIn, double yposIn);

	void ScrollMove(double yoffset);

private:

	void updateCameraVector();

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 WorldUp;
	glm::vec3 Up;
	glm::vec3 Right;

	float Yaw;
	float Pitch;
	float Speed;
	float Sensitivity;
	float Fov;

};