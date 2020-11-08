#pragma once

class Texture
{
private:
	unsigned int textureSlot;
	const char* textureImageFilePath;
public:
	Texture(const char* path, unsigned int slot);
	void bindToShader(unsigned int shader, const char* uniform);
};
