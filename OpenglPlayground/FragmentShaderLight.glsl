#version 400 core
out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPos;

in vec3 normal;
in vec3 FragWorldPos;

void main()
{
    vec3 normal = normalize(normal);
    vec3 lightDir = normalize(lightPosition - FragWorldPos);

    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * lightColor;

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    float specularStrength = 0.5;
    vec3 dirToCamera = normalize(cameraPos - FragWorldPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(dirToCamera, reflectDir), 0), 64);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 resultLighting = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(resultLighting, 1.0);
}