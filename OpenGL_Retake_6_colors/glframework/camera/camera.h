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
	
	glm::vec3 Position; //λ�á���������������������һ�����
	glm::vec3 Front;     
	glm::vec3 Up;      
	glm::vec3 Right;    //�������������������������������ƶ�
	glm::vec3 WorldUp;  //������������������涨�����������������

	float Yaw;   //������
	float Pitch; //ƫ����

	float moveSpeed;       //�ƶ��ٶ�
	float moveSensitivity; //�ӽ���ת������
	float Zoom;            //�佹�����ţ�fov

	void updateCameraVector();

};