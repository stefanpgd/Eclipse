#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPosition;

uniform vec3 cameraPosition;
uniform samplerCube skybox;

void main()
{             
    vec3 I = normalize(FragPosition - cameraPosition);
    vec3 R = reflect(I, normalize(Normal));
    //R = refract(I, normalize(Normal), 1.0 / 2.42);
    FragColor = vec4(texture(skybox, R).rgb, 1.0);

    // I = Incoming light, B - A, Camera to FragPosition
    // R = Reflecting the ray based on the Normal
    // Afterwards we can sample the skybox since the direction can be used as a texture coordinate.
}