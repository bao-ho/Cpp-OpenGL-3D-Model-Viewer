#pragma once

class MyTexture
{
private:
	unsigned int textureSlot;
	const char* textureImageFilePath;
public:
	MyTexture(const char* path, unsigned int slot);
	void bindToShader(unsigned int shader, const char* uniform);
};
