#version 460 core

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

in vec2 TexCoords;

out vec4 FragColor;

uniform Material u_Material;

void main()
{
    vec3 diffuse = texture(u_Material.texture_diffuse1, TexCoords).rgb;
    FragColor = vec4(diffuse, 1.0);
}
