#include "camera.h"

Camera::Camera(glm::vec3 &position, glm::vec3 &up, const float &pitch, const float &yaw) 
				: Front(glm::vec3(0.0, 0.0, -1.0)) , Speed(SPEED) , Sensitivity(Sensitivity) , Fov(FOV) {
	Position = position;
	WorldUp = up;
	Pitch = pitch;
	Yaw = yaw;

	updateCameraVector();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float pitch, float yaw) 
				: Front(glm::vec3(0.0, 0.0, -1.0)), Speed(SPEED), Sensitivity(Sensitivity), Fov(FOV) {
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;

	updateCameraVector();
}

Camera::~Camera() {}

void Camera::KeyMove(int dir, double deltaTime) {
	float velocity = deltaTime * Speed;
	
	if (dir == 1) Position += Front * velocity;
	if (dir == 2) Position -= Right * velocity;
	if (dir == 3) Position -= Front * velocity;
	if (dir == 4) Position += Right * velocity;
}

void Camera::MouseMove(double xoffset, double yoffset) {
	xoffset *= Sensitivity;
	yoffset *= Sensitivity;
	
	Yaw += xoffset;
	Pitch += yoffset;

	if (true) {
		if (Pitch > 89.0f) Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;
	}

	updateCameraVector();
}

void Camera::ScrollMove(double yoffset) {
	Fov -= yoffset;
	if (Fov > 45.0f) Fov = 45.0f;
	if (Fov < 1.0f) Fov = 1.0f;
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(Position, Position + Front, Up);
}

glm::vec3 Camera::getPostion() {
	return Position;
}

glm::vec3 Camera::getFront() {
	return Front;
}

glm::vec3 Camera::getWorldUp() {
	return WorldUp;
}

glm::vec3 Camera::getUp() {
	return Up;
}

glm::vec3 Camera::getRight() {
	return Right;
}

float Camera::getFov() {
	return Fov;
}

void Camera::updateCameraVector() {
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	Right = glm::normalize(glm::cross(Front, WorldUp));

	Up = glm::normalize(glm::cross(Right, Front));
}