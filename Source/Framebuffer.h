#pragma once

class Shader;

class Framebuffer
{
public:
	Framebuffer();

	void Bind();
    void Draw();

private:
    void GenerateScreenVAO();

private:
	unsigned int FBO; // Framebuffer Object
	unsigned int RBO; // Renderbuffer Object
	unsigned int colorBuffer; // texture for colorBuffer
    unsigned int screenVBO; // Vertex data for screen
    unsigned int screenVAO; // Quad VAO to draw to screen

    Shader* screenShader;

    float time = 0.0f;

    float screenVertices[24]
    {
        // Positions   // TexCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
};