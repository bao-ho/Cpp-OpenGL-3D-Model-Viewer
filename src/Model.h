#pragma once

#include <vector>
#include "Mesh.h"
#include <string>
//ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GL/glew.h>






class Model
{
private:
	std::vector<Texture> loadedTextures;
	std::vector<Mesh> meshes;
	std::string dir;
	bool gammaCorrection;

	void processNode(aiNode* node, aiScene const* scene);
	Mesh processMesh(aiMesh* mesh, aiScene const* scene);
	std::vector<Texture> loadMaterialTexture(aiMaterial* material, aiTextureType type, std::string typeName);
	unsigned int loadTextureFromFile(char const* path, const std::string& dir, bool gamma=false);
public:
	Model (const std::string & path, bool gamma=false);
	void Draw(unsigned int shaderID);
};