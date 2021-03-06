#pragma once

class Model;
class Shader;

class MeshRenderer
{
public:
	void Initialize();
	void Initialize(std::string fileName);

	void Draw(Shader* shader);

public:
	Model* model = nullptr;
	bool usesModel = false;

private:
	unsigned int VBO = 0;
	unsigned int VAO = 0;
	unsigned int vertexCount = 0;

	float cubeVertices[288] = {
		   -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		   -0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		   -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		   -0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
			0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
			0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
		   -0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
		   -0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,

		   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

			0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		   -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
			0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		   -0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		   -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

		   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	};

	bool meshSetup = false;
};