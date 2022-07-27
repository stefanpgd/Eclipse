#pragma once

class Shader;

class Framebuffer
{
public:
	Framebuffer();

	void Bind();
    void Draw();
    void BindShadowmaps();

private:
    void GenerateScreenVAO();

private:
	unsigned int FBO; // Framebuffer Object
	unsigned int RBO; // Renderbuffer Object
	unsigned int colorBuffer; // texture for colorBuffer
    unsigned int screenVBO; // Vertex data for screen
    unsigned int screenVAO; // Quad VAO to draw to screen

    // Shadow mapping
    unsigned int depthMap;
    unsigned int depthMapFBO;
    const unsigned int SHADOW_WIDTH = 4096;
    const unsigned int SHADOW_HEIGHT = 4096;

    Shader* screenShader;


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

    // Temp
    float time = 0.0f;
    vec2 rOffset = vec2(0.0);
    vec2 gOffset = vec2(0.0);
    vec2 bOffset = vec2(0.0);
    float vignetteStrength = 0.0f;
    float exposure = 1.0f;
};