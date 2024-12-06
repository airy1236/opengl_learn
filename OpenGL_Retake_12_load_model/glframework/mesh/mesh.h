#pragma once

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../shader/shader.h"

#define BONE_INDEX 4

struct Vertex {
	glm::vec3 Position; //λ��
	glm::vec3 Normal;   //����
	glm::vec2 TexCoords; //��������
	glm::vec3 Tangent;  //����(u����)
	glm::vec3 Bitangent;//������(v����)
	int m_BoneIDs[BONE_INDEX];
	float m_Weights[BONE_INDEX];
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public:

	//���죺���㣬����������
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	
	~Mesh();

	//��������ģ��
	void DrawShader(Shader shader);

private:

	void setupMesh();
	
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	GLuint VAO;
	GLuint VBO, EBO;
	
};