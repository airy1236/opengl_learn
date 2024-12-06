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
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
	int m_BoneIDs[BONE_INDEX];
	float m_Weights[BONE_INDEX];
};

struct mTexture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh {
public:

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<mTexture> textures);

	~Mesh();

	void DrawShader(Shader shader);

	GLuint getVAO();

	std::vector<unsigned int> getIndices();

private:

	void setupMesh();

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<mTexture> textures;
	GLuint VBO, VAO, EBO;

};