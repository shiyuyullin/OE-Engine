// Fragment Shader for lighting

#version 400 core

struct Material
{
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D emissionMap;
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
uniform bool useEmissonMap;

in vec3 normal;
in vec3 FragWorldPos;
in vec2 textureCoord;




void main()
{
    vec3 normal = normalize(normal);
    vec3 lightDir = normalize(light.lightPosition - FragWorldPos);

    vec3 ambient = light.ambient * vec3(texture(material.diffuseMap, textureCoord));

    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diffuseStrength * vec3(texture(material.diffuseMap, textureCoord));

    vec3 dirToCamera = normalize(cameraPos - FragWorldPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(dirToCamera, reflectDir), 0), 64);
    vec3 specular = light.specular * spec * vec3(texture(material.specularMap, textureCoord));

    vec3 emission;
    vec3 resultLighting;

    if(useEmissonMap)
    {
        emission = vec3(texture(material.emissionMap, textureCoord));
        resultLighting = ambient + diffuse + specular + emission;
    }
    else
    {
        resultLighting = ambient + diffuse + specular;
    }

    FragColor = vec4(resultLighting, 1.0);
}