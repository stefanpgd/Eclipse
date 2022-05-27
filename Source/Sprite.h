#pragma once

class Sprite
{
public:
	Sprite(std::string fileName);
	~Sprite();

	bool Initialize();

	int Width;
	int Height;
	int Channels;
	unsigned char* Buffer;

private:
	std::string location;
};