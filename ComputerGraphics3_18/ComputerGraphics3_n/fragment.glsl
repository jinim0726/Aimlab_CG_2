#version 330 core

in vec3 FragPos; 
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // Ambient lighting
    vec3 ambientLight = vec3(0.2);
    vec3 ambient = ambientLight * lightColor;

    // Diffuse lighting
    vec3 normalVector = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); // 광원 방향
    float diffuseLight = max(dot(normalVector, lightDir), 0.0);
    vec3 diffuse = diffuseLight * lightColor;

    // Combine results
    vec3 result = (ambient + diffuse) * objectColor;

    // Final output
    FragColor = vec4(result, 1.0);
}
