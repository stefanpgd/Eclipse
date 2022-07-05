#version 330 core
out vec4 FragColor;

in vec3 FragPosition;
in vec2 TextureCoord;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float materialShininess;
uniform vec3 cameraPosition;
uniform vec3 globalLightDirection;

void main()
{
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 lightAmbient = vec3(0.15, 0.15, 0.15);
	vec3 lightDiffuse = vec3(1.0, 1.0, 1.0);
	vec3 lightSpecular = vec3(1.0, 1.0, 1.0);

	vec3 ambientColor = vec3(0.0);
	vec3 diffuseColor = vec3(0.0);
	vec3 specularColor = vec3(0.0);

	vec4 diffuseTex = texture(texture_diffuse1, TextureCoord);

	if(diffuseTex.a <= 0.5)
	{
		discard;
	}

	// Ambient
	ambientColor = lightColor * lightAmbient * vec3(diffuseTex) * materialAmbient;

	// Diffuse
	float diffuseFactor = dot(-globalLightDirection, Normal);
	if(diffuseFactor > 0.0)
	{
		diffuseColor = vec3(diffuseTex) * diffuseFactor;
	}

	vec3 diff = lightColor * lightDiffuse * diffuseColor * materialDiffuse; 

	// Specular
	vec3 PixelToCamera = normalize(cameraPosition - FragPosition);
	vec3 reflectedLight = normalize(reflect(-globalLightDirection, Normal));
	float specularFactor = dot(reflectedLight, PixelToCamera);

	if(specularFactor > 0.0)
	{
		specularFactor = pow(specularFactor, materialShininess);
		vec4 specTex = texture(texture_specular1, TextureCoord) * specularFactor;
		specularColor = lightColor * lightSpecular * vec3(specTex) * materialSpecular; 
	}

	vec3 accumalted = ambientColor + diff + specularColor;
	FragColor = vec4(accumalted, 1.0);
}