#include "precomp.h"
#include "Framebuffer.h"
#include "Renderer.h"
#include "Shader.h"

Framebuffer::Framebuffer()
{
	screenShader = new Shader("FramebufferDraw.vert", "FramebufferDraw.frag");
	GenerateScreenVAO();

	// Generate framebuffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Generate color buffer texture
	glGenTextures(1, &colorBuffer);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, ScreenWidth, ScreenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
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

	// Generate Depth Map Framebuffer 
	glGenFramebuffers(1, &depthMapFBO);

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Bind()
{
	ImGui::Begin("Window 3");
	ImGui::Image((ImTextureID)depthMap, ImVec2(256, 256), ImVec2(0, 1), ImVec2(1,0));
	ImGui::End();

	ImGui::Begin("Placeholder");
	ImGui::DragFloat2("R - Offset", &rOffset[0], 0.00001f, -0.5, 0.5);
	ImGui::DragFloat2("G - Offset", &gOffset[0], 0.00001f, -0.5, 0.5);
	ImGui::DragFloat2("B - Offset", &bOffset[0], 0.00001f, -0.5, 0.5);
	ImGui::DragFloat("Vignette Strength", &vignetteStrength, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Exposure", &exposure, 0.01f, 0.1f, 50.0f);
	ImGui::End();

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glViewport(0, 0, ScreenWidth, ScreenHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, depthMap);
}

void Framebuffer::Draw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	screenShader->Use();
	screenShader->SetInt("screenTexture", 0);
	screenShader->SetInt("depthMap", 1);
	screenShader->SetVec2("rOffset", rOffset);
	screenShader->SetVec2("gOffset", gOffset);
	screenShader->SetVec2("bOffset", bOffset);
	screenShader->SetFloat("vignette", vignetteStrength);
	screenShader->SetFloat("exposure", exposure);

	glBindVertexArray(screenVAO);
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorBuffer);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Framebuffer::BindShadowmaps()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_FRONT);
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