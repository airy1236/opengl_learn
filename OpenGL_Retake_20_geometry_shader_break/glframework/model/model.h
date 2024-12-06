#pragma once

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../shader/shader.h"
#include "../mesh/mesh.h"

class Model {
public:

	Model(const std::string &path, bool gamma = true);

	~Model();

	void Draw(Shader shader);

private:

	std::vector<mTexture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	void loadModel(const std::string &path);

	void processNode(aiNode* node, const aiScene* scene);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<mTexture> loadMeterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	GLuint TextureFromFile(const char* path, const std::string &diectory, bool gamma = true);

};