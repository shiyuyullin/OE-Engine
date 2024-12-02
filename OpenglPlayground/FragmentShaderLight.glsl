// Fragment Shader for lighting

#version 400 core

struct Material
{
    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D emissionMap;
    float shininess;

};

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular; 

    float constant;
    float linear;
    float quadratic;

};

#define NR_POINT_LIGHTS 4



vec3 calcDirectionalLight(DirectionalLight dirLight, vec3 normal, vec3 dirToCamera);
vec3 calcPointLight(PointLight pointLight, vec3 normal, vec3 fragWorldPos, vec3 dirToCamera);

out vec4 FragColor;
  
uniform vec3 cameraPos;
uniform Material material;
uniform DirectionalLight directionalLight;
uniform bool useEmissonMap;
uniform PointLight pointLights[NR_POINT_LIGHTS];

in vec3 normal;
in vec3 FragWorldPos;
in vec2 textureCoord;




void main()
{

    vec3 normal = normalize(normal);
    vec3 dirToCamera = normalize(cameraPos - FragWorldPos);

    vec3 lightingOutput;
    
    lightingOutput = calcDirectionalLight(directionalLight, normal, dirToCamera);

    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        lightingOutput += calcPointLight(pointLights[i], normal, FragWorldPos, dirToCamera);
    }

    FragColor = vec4(lightingOutput, 1.0);
}


vec3 calcDirectionalLight(DirectionalLight dirLight, vec3 normal, vec3 dirToCamera)
{
    vec3 result;

    vec3 lightDir = normalize(-directionalLight.direction);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(dirToCamera, reflectDir), 0.0), material.shininess);
    
    vec3 ambient = dirLight.ambient * vec3(texture(material.diffuseMap, textureCoord));
    vec3 diffuse = dirLight.diffuse * diffuseStrength * vec3(texture(material.diffuseMap, textureCoord));
    vec3 specular = dirLight.specular * spec * vec3(texture(material.specularMap, textureCoord));

    if(useEmissonMap)
    {
        vec3 emission = vec3(texture(material.emissionMap, textureCoord));
        result = ambient + diffuse + specular + emission;
    }
    else
    {
        result = ambient + diffuse + specular;
    }

    return result;
}


vec3 calcPointLight(PointLight pointLight, vec3 normal, vec3 fragWorldPos, vec3 dirToCamera)
{
    vec3 result;

    vec3 lightDir = normalize(pointLight.position - FragWorldPos);
    float diffuseStrength = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(dirToCamera, reflectDir), 0), material.shininess);
    float distance = length(pointLight.position - FragWorldPos);
    float attenuation = 1.0 / (pointLight.constant + (pointLight.linear * distance) + (pointLight.quadratic * (distance * distance)));

    vec3 ambient = pointLight.ambient * vec3(texture(material.diffuseMap, textureCoord));
    vec3 diffuse = pointLight.diffuse * diffuseStrength * vec3(texture(material.diffuseMap, textureCoord));
    vec3 specular = pointLight.specular * spec * vec3(texture(material.specularMap, textureCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    if(useEmissonMap)
    {
        vec3 emission = vec3(texture(material.emissionMap, textureCoord));
        result = ambient + diffuse + specular + emission;
    }
    else
    {
        result = ambient + diffuse + specular;
    }

    return result;
}