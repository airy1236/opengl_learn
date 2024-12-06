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
	glm::vec3 Position; //位置
	glm::vec3 Normal;   //法线
	glm::vec2 TexCoords; //纹理坐标
	glm::vec3 Tangent;  //切线(u向量)
	glm::vec3 Bitangent;//副切线(v向量)
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

	//构造：顶点，索引，纹理
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	
	~Mesh();

	//绘制网格模型
	void DrawShader(Shader shader);

private:

	void setupMesh();
	
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	GLuint VAO;
	GLuint VBO, EBO;
	
};