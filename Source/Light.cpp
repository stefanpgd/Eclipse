#include "precomp.h"
#include "Light.h"
#include "Shader.h"
#include "Renderer.h"
#include "Camera.h"

Light::Light()
{
	ID = rand();
}

Light::Light(vec3 position, vec3 color, float intensity) :
	Position(position), Color(color), Intensity(intensity)
{
	ID = rand();
}

Light::Light(const Light& light)
{
	ID = rand();
	Name = light.Name;
	Intensity = light.Intensity;
	Position = light.Position;
	Color = light.Color;
	AmbientAmount = light.AmbientAmount;
	DiffuseAmount = light.DiffuseAmount;
	LinearFalloff = light.LinearFalloff;
	ExponentialFalloff = light.ExponentialFalloff;
	IsPointLight = light.IsPointLight;
	GlobalLightRotation = light.GlobalLightRotation;
}

glm::mat4& Light::GetLightSpaceMatrix()
{
	float near = n;
	float far = f;

	mat4 lightProjection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, near, far);
	mat4 lightView = glm::lookAt(testPosition, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));

	vec3 position = Camera::GetInstance()->CameraPosition + (normalize(-GlobalLightRotation) * height);
	vec3 target = Camera::GetInstance()->CameraPosition;

	if (tryOutDirectionalMethod)
	{
		// Directional light will follow the camera so that everything around cam will always be lit/shadow
		lightView = glm::lookAt(position, target, vec3(0.0f, 1.0f, 0.0f));
	}

	lightSpaceMatrix = lightProjection * lightView;
	return lightSpaceMatrix;
}

void Light::BindLightData(Shader* shader, unsigned int index)
{
	if (IsPointLight)
	{
		if (index > MaxNumberOfPointLights)
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
		GlobalLightRotation = normalize(GlobalLightRotation);

		shader->SetVec3("directionalLight.Color", Color);
		shader->SetVec3("directionalLight.Direction", GlobalLightRotation);
		shader->SetVec3("directionalLight.Ambient", AmbientAmount);
		shader->SetVec3("directionalLight.Diffuse", DiffuseAmount);
		shader->SetFloat("directionalLight.Intensity", Intensity);
	}
}