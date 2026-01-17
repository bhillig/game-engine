#version 460 core

struct Material
{
    sampler2D diffuse; // Diffuse texture
    sampler2D specular; // Specular texture
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

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;

    float innerCutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// struct Light
// {
//     int type;

//     vec3 position; // Used by: Point and spot lights
//     vec3 direction; // Used by: Directional and spot lights

//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;

//     // Point light attenuation parameters
//     float constant;
//     float linear;
//     float quadratic;

//     // Spotlight parameters
//     float innerCutOff;
//     float outerCutOff;
// };

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material u_Material;

// Light sources
uniform DirectionalLight u_DirectionalLight;
uniform PointLight[4] u_PointLights;
uniform SpotLight u_SpotLight;

uniform bool u_UseSpotLight;

uniform vec3 u_ViewPos;

vec3 CalculateDirectionalLighting(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    // Calculate ambient
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, TexCoords));

    // Calculate diffuse
    float diffuseStrength = max(dot(-light.direction, normal), 0.0);
    vec3 diffuse = (diffuseStrength * vec3(texture(u_Material.diffuse, TexCoords))) * light.diffuse;

    // Calculate specular
    vec3 reflectDirection = reflect(light.direction, normal);
    float specularStrength = pow(max(dot(reflectDirection, viewDir), 0.0), u_Material.shininess);
    vec3 specular = (specularStrength * vec3(texture(u_Material.specular, TexCoords))) * light.specular;

    return ambient + diffuse + specular;
}

vec3 CalculatePointLighting(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Direction from the frag to the point light
    vec3 lightDirection = normalize(light.position - fragPos);

    // Calculate ambient
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, TexCoords));

    // Calculate diffuse
    float diffuseStrength = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = (diffuseStrength * vec3(texture(u_Material.diffuse, TexCoords))) * light.diffuse;

    // Calculate specular
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularStrength = pow(max(dot(reflectDirection, viewDir), 0.0), u_Material.shininess);
    vec3 specular = (specularStrength * vec3(texture(u_Material.specular, TexCoords))) * light.specular;

    // Account for attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 CalculateSpotLighting(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // Direction from the frag to the spotlight
    vec3 lightDirection = normalize(light.position - fragPos);

    // Calculate ambient
    vec3 ambient = light.ambient * vec3(texture(u_Material.diffuse, TexCoords));

    // Calculate diffuse
    float diffuseStrength = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = (diffuseStrength * vec3(texture(u_Material.diffuse, TexCoords))) * light.diffuse;

    // Calculate specular
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularStrength = pow(max(dot(reflectDirection, viewDir), 0.0), u_Material.shininess);
    vec3 specular = (specularStrength * vec3(texture(u_Material.specular, TexCoords))) * light.specular;

    // Account for intensity
    float theta = dot(-light.direction, lightDirection);
    float epsilon = light.innerCutOff - light.outerCutOff;
    float intensity = (theta - light.outerCutOff) / epsilon;
    intensity = clamp(intensity, 0.f, 1.f);

    diffuse *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular;
}

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(u_ViewPos - FragPos);

    // Perform directional lighting calculations
    vec3 color = CalculateDirectionalLighting(u_DirectionalLight, normal, viewDir);

    // Perform point light calculations
    for (int i = 0; i < 4; ++i)
    {
        color += CalculatePointLighting(u_PointLights[i], normal, FragPos, viewDir);
    }

    // Perform spot light calculations
    if (u_UseSpotLight)
    {
        color += CalculateSpotLighting(u_SpotLight, normal, FragPos, viewDir);
    }

    FragColor = vec4(color, 1.0);
}
