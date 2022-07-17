#include "precomp.h"
#include "Framebuffer.h"
#include "Renderer.h"
#include "Shader.h"

Framebuffer::Framebuffer()
{
	// Generate framebuffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Generate color buffer texture
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ScreenWidth, ScreenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Attach color buffer to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

	// Generate Renderbuffer 
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, ScreenWidth, ScreenHeight);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// Check status
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		Renderer::GetInstance()->ConsoleLog("Framebuffer creation went wrong, framebuffer isn't complete!", WarningLevel::Error);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GenerateScreenVAO();

	screenShader = new Shader("FramebufferDraw.vert", "FramebufferDraw.frag");
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void Framebuffer::Draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	screenShader->Use();
	glBindVertexArray(screenVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Framebuffer::GenerateScreenVAO()
{
	glGenVertexArrays(1, &screenVAO);
	glGenBuffers(1, &screenVBO);

	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), &screenVertices[0], GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);

	// Texture Coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));

	glBindVertexArray(0);
}