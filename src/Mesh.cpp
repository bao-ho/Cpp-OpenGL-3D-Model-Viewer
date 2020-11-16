#include "Mesh.h"
#include <GL/glew.h>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	glGenVertexArrays(1, &vertexArrayID);
	glGenBuffers(1, &vertexBufferID);
	glGenBuffers(1, &elementBufferID);

	glBindVertexArray(vertexArrayID);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);//Normal
	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);//TextureCoordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, TextureCoordinates));
	glEnableVertexAttribArray(3);//Tangent
	glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	glEnableVertexAttribArray(4);//Bitangent
	glVertexAttribPointer(4, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::Draw(unsigned int shaderID)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string textureUnit;
		if (textures[i].type == "texture_diffuse")
			textureUnit = "texture_diffuse" + std::to_string(diffuseNr++);
		else if (textures[i].type == "texture_specular")
			textureUnit = "texture_specular" + std::to_string(specularNr++);
		else if (textures[i].type == "texture_normal")
			textureUnit = "texture_normal" + std::to_string(normalNr++);
		else if (textures[i].type == "texture_height")
			textureUnit = "texture_height" + std::to_string(heightNr++);

		glUniform1i(glGetUniformLocation(shaderID, textureUnit.c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
	}
	glBindVertexArray(vertexArrayID);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(vertexArrayID);

	glActiveTexture(0);
}
