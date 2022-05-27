#pragma once

class Sprite;

class Texture
{
public:
	Texture(std::string fileName);

	bool Initialize();

	unsigned int ID;
	int Width = 0;
	int Height = 0;
	int Channels = 0;

private:
	Sprite* image;
	std::string file;
	std::string location;
};