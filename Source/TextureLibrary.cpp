#include "precomp.h"
#include "Texture.h"
#include "TextureLibrary.h"

Texture* TextureLibrary::GetTexture(std::string location)
{
	Texture* texture = textures[location];

	if (texture == nullptr)
	{
		texture = new Texture(location);
		textures[location] = texture;
	}

	return texture;
}