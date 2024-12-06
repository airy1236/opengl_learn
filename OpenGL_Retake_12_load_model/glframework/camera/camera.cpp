#include "camera.h"

Camera::Camera(glm::vec3 &position, glm::vec3 &up, const float &yaw, const float &pitch) 
	: Front(glm::vec3(0.0, 0.0, -1.0)) , Speed(SPEED) , Sensitivity(SENSITIVITY) , Fov(FOV) {
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;

	updateCameraVector();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: Front(glm::vec3(0.0, 0.0, -1.0)), Speed(SPEED), Sensitivity(SENSITIVITY), Fov(FOV) {
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;

	updateCameraVector();
}

Camera::~Camera() {}

void Camera::KeyMove(std::string direction, float deltaTime) {
	float velocity = Speed * deltaTime;
	
	if (direction == "FORWARD") Position += Front * Sensitivity;
	if (direction == "BACKWARD") Position -= Front * Sensitivity;
	if (direction == "RIGHT") Position += Right * Sensitivity;
	if (direction == "LEFT") Position -= Right * Sensitivity;
}

void Camera::MouseMove(float xoffset, float yoffset, GLboolean constrianPitch) {
	xoffset *= Sensitivity;
	yoffset *= Sensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constrianPitch) {
		if (Pitch > 89.0f) Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;
	}

	updateCameraVector();
}

void Camera::MouseScroll(float yoffset) {
	Fov -= yoffset;

	if (Fov > 45.0f) Fov = 45.0f;
	if (Fov < 1.0f) Fov = 1.0f;
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(Position, Position + Front, Up);
}

glm::vec3 Camera::getPosition() {
	return Position;
}

glm::vec3 Camera::getFront() {
	return Front;
}

glm::vec3 Camera::getWorldUp() {
	return WorldUp;
}

glm::vec3 Camera::getRight() {
	return Right;
}

glm::vec3 Camera::getUp() {
	return Up;
}

float Camera::getYaw() {
	return Yaw;
}

float Camera::getPitch() {
	return Pitch;
}

float Camera::getSpeed() {
	return Speed;
}

float Camera::getSensitivity() {
	return Sensitivity;
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