#pragma once

#include <iostream>
#include <map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



#include "../assimp_glm_helpers/assimp_glm_helpers.h"
#include "../shader/shader.h"
#include "../mesh/mesh.h"

struct BoneInfo {
	int id;
	glm::mat4 offset;
};

class Model {
public:

	Model(const std::string &path, bool gamma = true);

	~Model();

	void Draw(Shader shader);

	std::vector<Mesh> getMeshes();

	std::vector<mTexture> getTextures_Loaded();

	std::map<std::string, BoneInfo>& GetBoneInfoMap();
	int& GetBoneCount();

private:

	std::map<std::string, BoneInfo> m_BoneInfoMap;
	int m_BoneCounter = 0;

	std::vector<mTexture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gammaCorrection;

	void loadModel(const std::string &path);

	void processNode(aiNode* node, const aiScene* scene);

	void SetVertexBoneDataToDefault(Vertex& vertex);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	void SetVertexBoneData(Vertex& vertex, int boneID, float weight);

	void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

	std::vector<mTexture> loadMeterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	GLuint TextureFromFile(const char* path, const std::string &diectory, bool gamma = true);

};