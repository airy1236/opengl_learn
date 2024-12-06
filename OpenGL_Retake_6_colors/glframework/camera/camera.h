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
const float ZOOM = 45.0f;

class Camera {
public:

	Camera(glm::vec3& position = glm::vec3(0.0, 0.0, 0.0), glm::vec3& up = glm::vec3(0.0, 1.0, 0.0),
		   const float& yaw = YAW, const float& pitch = PITCH);

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	~Camera();

	glm::mat4 getViewMatrix();

	void PressKey();

	void KeyMove(std::string direction, float deltaTime);

	void MouseMove(float xoffset, float yoffset, GLboolean constrianPitch = true);

	void MouseSroll(float yoffset);

	float getZoom() { return Zoom; }

private:
	
	glm::vec3 Position; //位置、方向、上向量，用来定义一个相机
	glm::vec3 Front;     
	glm::vec3 Up;      
	glm::vec3 Right;    //相机的右向量，用来计算相机的左右移动
	glm::vec3 WorldUp;  //世界坐标的上向量，规定了世界坐标的正方向

	float Yaw;   //俯仰角
	float Pitch; //偏航角

	float moveSpeed;       //移动速度
	float moveSensitivity; //视角旋转灵敏度
	float Zoom;            //变焦（缩放）fov

	void updateCameraVector();

};