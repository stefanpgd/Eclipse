#include "precomp.h"
#include "Sprite.h"
#include "Texture.h"

Texture::Texture(std::string fileName)
{
	image = new Sprite(fileName);
	
	Width = image->Width;
	Height = image->Height;
	Channels = image->Channels;

	bool textureSetup = Initialize();

	if (!textureSetup)
	{
		throw std::runtime_error("ERROR: Failed to setup texture with file: " + fileName);
	}
}

bool Texture::Initialize()
{
	// Generate a texture ID to use
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	// Texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (image->Buffer)
	{
		// Generate texture with correct format
		unsigned int format = image->Channels == 3 ? GL_RGB : GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, image->Width, image->Height, 0, format, GL_UNSIGNED_BYTE, image->Buffer);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		return false;
	}

	delete image;
	return true;
}