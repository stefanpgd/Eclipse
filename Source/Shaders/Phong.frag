#version 330 core
out vec4 FragColor;

#define MAX_POINT_LIGHTS 16

in vec3 FragPosition;
in vec2 TextureCoord;
in vec3 Normal;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform vec3 cameraPosition;

struct Material
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
};

struct DirectionalLight
{
    vec3 Color;
    vec3 Direction;
    vec3 Ambient;
    vec3 Diffuse;
};

struct PointLight
{
    vec3 Position;
    vec3 Color;
    vec3 Ambient;
    vec3 Diffuse;

    float Intensity;
    float LinearFalloff;
    float ExponentialFalloff;
};

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform Material material;
uniform int amountOfPointLights;

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 Normal, vec3 viewDirection, vec3 diffuseTex, vec3 specularTex);
vec3 CalculatePointLight(PointLight light, vec3 Normal, vec3 viewDirection, vec3 diffuseTex, vec3 specularTex);

void main()
{
	vec4 diffuseTex = texture(texture_diffuse, TextureCoord);
    vec4 specularTex = texture(texture_specular, TextureCoord);

	if(diffuseTex.a <= 0.5)
	{
		discard;
	}

    vec3 normal =  normalize(Normal);
    vec3 viewDirection = normalize(cameraPosition - FragPosition);

    vec3 result;

    if(directionalLight.Color != 0)
    {
        result += CalculateDirectionalLight(directionalLight, normal, viewDirection, vec3(diffuseTex), vec3(specularTex));
    }

    for(int i = 0; i < amountOfPointLights; i++)
    {
        result += CalculatePointLight(pointLights[i], normal, viewDirection, vec3(diffuseTex), vec3(specularTex));
    }

    result = clamp(result, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));
    FragColor = vec4(result, 1.0);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 norm, vec3 viewDirection, vec3 diffuseTex, vec3 specularTex)
{
    vec3 ambient = light.Color * light.Ambient * diffuseTex * material.Ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 lightDirection = normalize(-light.Direction);
    float diffuseFactor = dot(lightDirection, norm);

    if(diffuseFactor > 0.0)
    {
        diffuse = light.Color * light.Diffuse * diffuseFactor * diffuseTex * material.Diffuse;

        vec3 halfwayDirection = normalize(lightDirection + viewDirection);
        float specularFactor = dot(norm, halfwayDirection);

        if(specularFactor > 0.0)
        {
            float specularExponent = pow(specularFactor, material.Shininess);
            specular = light.Color * specularExponent * specularTex.r;
        }
    }

    vec3 result = ambient + diffuse + specular;
    return result;
}

vec3 CalculatePointLight(PointLight light, vec3 norm, vec3 viewDirection, vec3 diffuseTex, vec3 specularTex)
{
    vec3 lightDirection = normalize(light.Position - FragPosition);
    vec3 ambient = light.Color * light.Ambient * diffuseTex * material.Ambient;
    vec3 diffuse;
    vec3 specular;

    float diffuseFactor = dot(lightDirection, norm);

    if(diffuseFactor > 0.0)
    {
        diffuse = light.Color * light.Diffuse * diffuseFactor * diffuseTex * material.Diffuse;

        vec3 halfwayDirection = normalize(lightDirection + viewDirection);
        float specularFactor = dot(norm, halfwayDirection);

        if(specularFactor > 0.0)
        {
            float specularExponent = pow(specularFactor, material.Shininess);
            specular = light.Color * specularExponent * specularTex.r;
        }
    }

    float lightDistance = length(light.Position - FragPosition);
    float attentuation = 1.0 / (1.0 + (light.LinearFalloff * lightDistance) + (light.ExponentialFalloff * lightDistance * lightDistance));
    float intensity = light.Intensity * attentuation;

    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;

    vec3 result = ambient + diffuse + specular;
    return result;
}