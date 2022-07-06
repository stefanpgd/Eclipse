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
uniform vec3 globalLightColor;

void main()
{
	vec3 lightColor = globalLightColor;
	vec3 lightAmbient = vec3(0.10, 0.10, 0.10);
	vec3 lightDiffuse = vec3(1.0, 1.0, 1.0);

//	vec3 ambientColor = vec3(0.0);
//	vec3 diffuseColor = vec3(0.0);
//	vec3 specularColor = vec3(0.0);
	vec4 diffuseTex = texture(texture_diffuse1, TextureCoord);

	if(diffuseTex.a <= 0.5)
	{
		discard;
	}
//
//	// Ambient
//	ambientColor = lightColor * lightAmbient * vec3(diffuseTex) * materialAmbient;
//
//	// Diffuse
//	float diffuseFactor = dot(-globalLightDirection, Normal);
//	if(diffuseFactor > 0.0)
//	{
//		diffuseColor = vec3(diffuseTex) * diffuseFactor;
//
//		// Specular
//		vec3 PixelToCamera = normalize(cameraPosition - FragPosition);
//		vec3 reflectedLight = normalize(reflect(-globalLightDirection, Normal));
//		float specularFactor = dot(PixelToCamera, reflectedLight);
//
//		if(specularFactor > 0.0)
//		{
//			float specularExponent = texture2D(texture_specular1, TextureCoord).r * 255.0;
//			specularFactor = pow(specularFactor, specularExponent);
//			specularColor = lightColor * specularFactor;
//		}
//	}
//
//	vec3 diff = lightColor * lightDiffuse * diffuseColor * materialDiffuse; 
//
//	vec3 accumalted = ambientColor + diff + specularColor;
//	FragColor = vec4(accumalted, 1.0);

    vec3 pointLightDirection = FragPosition - cameraPosition;
    float pointLightDistance = length(cameraPosition - FragPosition);
    pointLightDirection = normalize(pointLightDirection);
    float attentuation = 1.0 / (1.0 + (0.02 * pointLightDistance) + (0.0 * pointLightDistance * pointLightDistance));
    float intensity = 10.0 * attentuation;

    vec3 normal =  normalize(Normal);
    vec3 viewDirection = normalize(cameraPosition - FragPosition);
    vec3 lightDirection = normalize(-pointLightDirection);
    
    // Diffuse Shading
    float diffuseAmount = max(dot(normal, lightDirection), 0.0);
    
    // Specular shading
    vec3 reflectDirection = reflect(-lightDirection, normal);
    vec3 halfwayDir = normalize(viewDirection + lightDirection);
    float specularAmount = pow(max(dot(halfwayDir, normal), 0.0), materialShininess);

    // Result
    vec3 ambient = lightColor * lightAmbient * vec3(texture(texture_diffuse1, TextureCoord)) * materialAmbient;
    vec3 diffuse = lightColor * lightDiffuse * diffuseAmount * vec3(texture(texture_diffuse1, TextureCoord)) * materialDiffuse;
    vec3 specular = lightColor * specularAmount * vec3(texture(texture_specular1, TextureCoord).r) * materialSpecular;

    ambient *= intensity;
    diffuse *= intensity;
    specular *= intensity;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}