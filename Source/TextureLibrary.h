#pragma once

class Texture;

#include <map>

class TextureLibrary
{
public:
	static TextureLibrary* GetInstance()
	{
		static TextureLibrary instance;
		return &instance;
	}

	Texture* GetTexture(std::string location);

private:
	std::map<std::string, Texture*> textures;
};