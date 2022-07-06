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
	vec3 lightDiffuse = vec3(0.8, 0.8, 0.8);
	vec3 lightSpecular = vec3(0.3,0.3, 0.3);
//
//	vec3 ambientColor = vec3(0.0);
//	vec3 diffuseColor = vec3(0.0);
//	vec3 specularColor = vec3(0.0);
//
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


    vec3 normal =  normalize(Normal);
    vec3 viewDirection = normalize(cameraPosition - FragPosition);
    vec3 lightDirection = normalize(-globalLightDirection);
    
    // Diffuse Shading
    float diffuseAmount = max(dot(normal, lightDirection), 0.0);
    
    // Specular shading
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularAmount = pow(max(dot(viewDirection, reflectDirection), 0.0), materialShininess);

    // Result
    vec3 ambient = lightColor * lightAmbient * vec3(texture(texture_diffuse1, TextureCoord)) * materialAmbient;
    vec3 diffuse = lightColor * lightDiffuse * diffuseAmount * vec3(texture(texture_diffuse1, TextureCoord)) * materialDiffuse;
    vec3 specular = lightColor * lightSpecular * specularAmount * vec3(texture(texture_specular1, TextureCoord).r);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}