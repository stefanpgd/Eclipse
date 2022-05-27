#include "precomp.h"
#include "stb_image.h"
#include "Sprite.h"

Sprite::Sprite(std::string fileName)
{
	location = "Assets/Textures/" + fileName;
	bool spriteSetup = Initialize();

	if (!spriteSetup)
	{
		throw std::runtime_error("ERROR: Failed to create/load sprite: " + fileName);
	}
}

Sprite::~Sprite()
{
	stbi_image_free(Buffer);
}

bool Sprite::Initialize()
{
	stbi_set_flip_vertically_on_load(true);
	Buffer = stbi_load(location.c_str(), &Width, &Height, &Channels, 0);
	return Buffer;
}