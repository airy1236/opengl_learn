#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 0.01f;
const float SENSITIVITY = 0.1f;
const float FOV = 45.0f;

class Camera {
public:

	Camera(glm::vec3 &position = glm::vec3(0.0, 0.0, 0.0), glm::vec3 &up = glm::vec3(0.0, 1.0, 0.0), const float &yaw = YAW, const float &pitch = PITCH);

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	~Camera();

	void KeyMove(std::string direction, float deltaTime);

	void MouseMove(float xposIn, float yposIn, GLboolean constrianPitch = true);

	void MouseScroll(float yoffset);

	glm::mat4 getViewMatrix();
	
	glm::vec3 getPosition();

	glm::vec3 getFront();

	glm::vec3 getWorldUp();

	glm::vec3 getRight();

	glm::vec3 getUp();

	float getYaw();

	float getPitch();

	float getSpeed();

	float getSensitivity();

	float getFov();	

private:

	void updateCameraVector();

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 WorldUp;
	glm::vec3 Right;
	glm::vec3 Up;

	float Yaw;
	float Pitch;
	float Speed;
	float Sensitivity;
	float Fov;

};