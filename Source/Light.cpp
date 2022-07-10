#include "precomp.h"
#include "Light.h"
#include "Shader.h"
#include "Renderer.h"

Light::Light()
{
	ID = rand();
}

Light::Light(vec3 position, vec3 color, float intensity) :
	Position(position), Color(color), Intensity(intensity)
{
	ID = rand();
}

void Light::BindLightData(Shader* shader, unsigned int index)
{
	if (IsPointLight)
	{
		if (index >= MaxNumberOfPointLights)
		{
			Renderer::GetInstance()->ConsoleLog("Exceeding max amount of point lights", WarningLevel::Error);
			return;
		}

		std::string arrayLocation = "pointLights[" + std::to_string(index - 1) + "].";

		shader->SetFloat(arrayLocation + "Intensity", Intensity);
		shader->SetFloat(arrayLocation + "LinearFalloff", LinearFalloff);
		shader->SetFloat(arrayLocation + "ExponentialFalloff", ExponentialFalloff);

		shader->SetVec3(arrayLocation + "Position", Position);
		shader->SetVec3(arrayLocation + "Color", Color);
		shader->SetVec3(arrayLocation + "Ambient", AmbientAmount);
		shader->SetVec3(arrayLocation + "Diffuse", DiffuseAmount);
	}
	else
	{
		shader->SetVec3("directionalLight.Color", Color);
		shader->SetVec3("directionalLight.Direction", GlobalLightRotation);
		shader->SetVec3("directionalLight.Ambient", AmbientAmount);
		shader->SetVec3("directionalLight.Diffuse", DiffuseAmount);
	}
}