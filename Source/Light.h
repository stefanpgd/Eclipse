#pragma once

class Shader;

class Light
{
public:
	Light();
	Light(vec3 position, vec3 color, float intensity);

	void BindLightData(Shader* shader, unsigned int index);

public:
	std::string Name = "Light";
	unsigned int ID;

	float Intensity = 10.0f;
	vec3 Position;
	vec3 Color = vec3(0.0);
	vec3 AmbientAmount = vec3(0.1f);
	vec3 DiffuseAmount = vec3(1.0f);
	float LinearFalloff = 0.02f;
	float ExponentialFalloff = 0.01f;

	bool IsPointLight = true;
	vec3 GlobalLightRotation; // only relevant when Light is a 'Global Light/Directional Light'
};