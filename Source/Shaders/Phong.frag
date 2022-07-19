#version 330 core
out vec4 FragColor;

#define MAX_POINT_LIGHTS 16

in vec3 FragPosition;
in vec2 TextureCoord;
in vec3 Normal;
in vec4 FragPosLightSpace;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D shadowMap;

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

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 Normal, vec3 viewDirection, vec3 diffuseTex, vec3 specularTex, float shadow);
vec3 CalculatePointLight(PointLight light, vec3 Normal, vec3 viewDirection, vec3 diffuseTex, vec3 specularTex);
float ShadowCalculation(vec4 fragPosLightSpace);

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
    float shadow = ShadowCalculation(FragPosLightSpace);

//    if(shadow == 1.0)
//    {
//        FragColor = vec4(0.8, 0.2, 0.1, 1.0);
//        return;
//    }

    if(directionalLight.Color != 0)
    {
        result += CalculateDirectionalLight(directionalLight, normal, viewDirection, vec3(diffuseTex), vec3(specularTex), shadow);
    }

    for(int i = 0; i < amountOfPointLights; i++)
    {
        result += CalculatePointLight(pointLights[i], normal, viewDirection, vec3(diffuseTex), vec3(specularTex));
    }

    result = clamp(result, vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0));
    FragColor = vec4(result, 1.0);
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/2.2));
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 norm, vec3 viewDirection, vec3 diffuseTex, vec3 specularTex, float shadow)
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
            specular = light.Color * (specularExponent * specularTex.r);
        }
    }

    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular));
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
            specular = light.Color * (specularExponent * specularTex.r);
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

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projectedFrag = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projectedFrag = projectedFrag * 0.5 + 0.5;

    if(projectedFrag.z > 1.0)
    {
        return 0.0;
    }

    float currentDepth = projectedFrag.z;
    float storedDepth = texture(shadowMap, projectedFrag.xy).r;

    float shadowBias = max(0.05 * (1.0 - dot(Normal, directionalLight.Direction)), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projectedFrag.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - shadowBias > pcfDepth ? 1.0 : 0;
        }
     }

     shadow /= 9.0;

     return shadow;


    //return currentDepth - shadowBias > storedDepth ? 1.0 : 0.0;
}