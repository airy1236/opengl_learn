#include "model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

Model::Model(const std::string &path, bool gamma) : gammaCorrection(gamma) {
	loadModel(path);
}

Model::~Model() {}

void Model::Draw(Shader shader) {
	for (int i = 0; i < meshes.size(); ++i) meshes[i].DrawShader(shader);
}

std::vector<Mesh> Model::getMeshes() {
	return meshes;
}

std::vector<mTexture> Model::getTextures_Loaded() {
	return textures_loaded;
}

std::map<std::string, BoneInfo>& Model::GetBoneInfoMap() {
	return m_BoneInfoMap;
}

int& Model::GetBoneCount() {
	return m_BoneCounter;
}

void Model::loadModel(const std::string &path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
	aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Assimp error\n" << importer.GetErrorString() << std::endl;
		return;
	}
	
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; ++i) {
		processNode(node->mChildren[i], scene);
	}
}

void Model::SetVertexBoneDataToDefault(Vertex& vertex) {
	for (int i = 0; i < BONE_INDEX; ++i) {
		vertex.m_BoneIDs[i] = -1;
		vertex.m_Weights[i] = 0.0f;
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<mTexture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		Vertex vertex;
		SetVertexBoneDataToDefault(vertex);
		vertex.Position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
		vertex.Normal = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);
		
		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else vertex.TexCoords = glm::vec2(0.0, 0.0);

		vertices.emplace_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j) {
			indices.emplace_back(face.mIndices[j]);
		}
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	std::vector<mTexture> diffuseMaps = loadMeterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.begin(), diffuseMaps.begin(), diffuseMaps.end());
	
	std::vector<mTexture> specularMaps = loadMeterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.begin(), specularMaps.begin(), specularMaps.end());
	
	std::vector<mTexture> normalMaps = loadMeterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.begin(), normalMaps.begin(), normalMaps.end());
	
	std::vector<mTexture> heightMaps = loadMeterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.begin(), heightMaps.begin(), heightMaps.end());

	ExtractBoneWeightForVertices(vertices, mesh, scene);

	return Mesh(vertices, indices, textures);
}

void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight) {
	for (int i = 0; i < BONE_INDEX; ++i) {
		if (vertex.m_BoneIDs[i] < 0) {
			vertex.m_BoneIDs[i] = boneID;
			vertex.m_Weights[i] = weight;
			break;
		}
	}
}

void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene) {
	auto& boneInfoMap = m_BoneInfoMap;
	int& boneCount = m_BoneCounter;

	for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
		int boneID = -1;
		std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
		if (boneInfoMap.find(boneName) == boneInfoMap.end()) {
			BoneInfo newBoneInfo;
			newBoneInfo.id = boneCount;
			newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
			boneInfoMap[boneName] = newBoneInfo;
			boneID = boneCount;
			boneCount++;
		}
		else boneID = boneInfoMap[boneName].id;
		
		assert(boneID != -1);
		
		auto weights = mesh->mBones[boneIndex]->mWeights;
		int numWeights = mesh->mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex) {
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;
			assert(vertexId <= vertices.size());
			SetVertexBoneData(vertices[vertexId], boneID, weight);
		}
	}
}

std::vector<mTexture> Model::loadMeterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<mTexture> textures;
	
	for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i) {
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for (int j = 0; j < textures_loaded.size(); ++j) {
			if (strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
				textures.emplace_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip) {
			mTexture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.emplace_back(texture);
			textures_loaded.emplace_back(texture);
		}
	}

	return textures;
}

GLuint Model::TextureFromFile(const char* path, const std::string& directory, bool gamma) {
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	GLuint texture;
	glGenTextures(1, &texture);

	int width, height, channels;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
	if (data) {
		GLenum format;
		if (channels == 1) format = GL_RED;
		else if (channels == 3) format = GL_RGB;
		else if (channels == 4) format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return texture;
}