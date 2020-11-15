#pragma once

#include <vector>
#include "Mesh.h"
#include <string>
//ASSIMP
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#include <GL/glew.h>



class Model
{
private:
	vector<Texture> loadedTextures;
	vector<Mesh> meshes;
	std::string dir;
	bool gammaCorrection;

	void processNode(aiNode* node, aiScene const* scene);
	void processMesh(aiMesh* mesh, aiScene const* scene);
	vector<Texture> loadMaterialTexture(aiMaterial* material, aiTextureType type, std::string typeName);
	unsigned int loadTextureFromFile(char const* path, string const& dir, bool gamma);
public:
	Model (std::string const& path, bool gammaCorrection);
};