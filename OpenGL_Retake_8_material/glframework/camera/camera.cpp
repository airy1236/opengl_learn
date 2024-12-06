#include "camera.h"

Camera::Camera(glm::vec3 &position, glm::vec3 &up, const float &yaw, const float &pitch)
	: Front(glm::vec3(0.0, 0.0, -1.0)) , Speed(SPEED) , Sensitivity(SENSITIVITY), Fov(FOV) {
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;

	updateCameraVector();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
	: Front(glm::vec3(0.0, 0.0, -1.0)), Speed(SPEED) , Sensitivity(SENSITIVITY), Fov(FOV) {
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;

	updateCameraVector();
}

Camera::~Camera() {}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(Position, Position + Front, Up);
}

glm::vec3 Camera::getPosition() {
	return Position;
}

float Camera::getFov() {
	return Fov;
}

void Camera::KeyMove(const std::string direction, float deltaTime) {
	float velocity = deltaTime * Speed;

	if (direction == "FORWARD") Position += Front * velocity;
	if (direction == "BACKWARD") Position -= Front * velocity;
	if (direction == "RIGHT") Position += Right * velocity;
	if (direction == "LEFT") Position -= Right * velocity;
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
	if (Fov < 1.0f) Fov = 1.0f;
	if (Fov > 45.0f) Fov = 45.0f;
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