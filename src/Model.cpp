#include "Model.h"
#include <iostream>
#include "Mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


void Model::processNode(aiNode* node, aiScene const* scene)
{
	for (unsigned int i; i < node.mNumMeshse; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], scene);
}

void Model::processMesh(aiMesh* mesh, aiScene const* scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	//VERTICES
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		if (mesh->HasNormals())
		{
			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;
		}
		if (mesh->TextureCoordinates[0])
		{
			vertex.TextureCoordinates.x = mesh->mTextureCoords[0][i].x;
			vertex.TextureCoordinates.y = mesh->mTextureCoords[0][i].y;
			vertex.Tangent.x = mesh->mTangents[i].x;
			vertex.Tangent.y = mesh->mTangents[i].y;
			vertex.Tangent.z = mesh->mTangents[i].z;
			vertex.Bitangent.x = mesh->mBitangents[i].x;
			vertex.Bitangent.y = mesh->mBitangents[i].y;
			vertex.Bitangent.z = mesh->mBitangents[i].z;
		}
		else
			vertex.TextureCoordinates = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}

	//FACES
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	//MATERIALS
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	vector<Texture> diffuseMaps = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	vector<Texture> specularMaps = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end()); 
	vector<Texture> normalMaps = loadMaterialTexture(material, aiTextureType_NORMAL, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	vector<Texture> heightMaps = loadMaterialTexture(material, aiTextureType_HEIGHT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return Mesh(vertices, indices, textures);
}

vector<Texture> Model::loadMaterialTexture(aiMaterial* material, aiTextureType type, std::string typeName)
{
	vector<Texture> texture;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < loadedTextures.size(); j++)
		{
			if (std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}
	}
	if (!skip)
	{
		Texture texture;
		texture.ID = loadTextureFromFile(str.C_str(), this->dir);
		texture.type = typeName;
		texture.path = str.C_str();
		textures.push_back(texture);
		loadedTextures.push_back(texture);
	}
	return texture;
}

unsigned int Model::loadTextureFromFile(char const* path, std::string const& dir, bool gamma)
{
	std::string fileName = std::string(path);
	fileName = dir + '/' + fileName;
	unsigned int width, height, nComponents;
	unsigned char* data = stbi_load(fileName, &width, &height, &nComponents, 0);
	unsigned int textureID;
	glGenTextures(1, &textureID);
	if (data)
	{
		GLenum format;
		if (nComponents == 1)
			format = GL_RED;
		else if (nComponents == 3)
			format = GL_RGB;
		else if (nComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
		std::cout << "Cannot load texture at path: " << path << std::endl;
	stbi_image_free(data);
}

Model::Model(std::string const& path, bool gammaCorrection)
{
	Assimp::Importer importer;
	aiScene const* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
	{
		std::cout << "Error: ASSIMP::" << importer.getErrorString() << endl;
		return;	
	}
	dir = path.substr(0, path.find_last_of('/'));
}
