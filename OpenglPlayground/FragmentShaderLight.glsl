#version 400 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

};

struct Light
{
    vec3 lightPosition;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};

out vec4 FragColor;
  
uniform vec3 cameraPos;

uniform Material material;
uniform Light light;

in vec3 normal;
in vec3 FragWorldPos;




void main()
{
    vec3 normal = normalize(normal);
    vec3 lightDir = normalize(light.lightPosition - FragWorldPos);

    vec3 ambient = light.ambient * material.ambient;

    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = (diffuseStrength * material.diffuse) * light.diffuse;

    vec3 dirToCamera = normalize(cameraPos - FragWorldPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(dirToCamera, reflectDir), 0), 64);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 resultLighting = ambient + diffuse + specular;

    FragColor = vec4(resultLighting, 1.0);
}