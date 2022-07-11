#pragma once

class Shader;

class Skybox
{
public:
    Skybox(std::string skyboxFolder);

	void Draw();

private:
    void CreateVAO();

	unsigned int ID;
    unsigned int VBO;
    unsigned int VAO;

    Shader* shader;

	std::string faces[6]
	{
		"right.png",
		"left.png",
		"top.png",
		"bottom.png",
		"front.png",
		"back.png"
	};
};