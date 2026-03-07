#version 460 core

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

in vec2 v_TexCoords;

out vec4 FragColor;

uniform Material u_Material;
uniform vec4 u_Color;
uniform float u_TilingFactor;

void main()
{
    FragColor = texture(u_Material.texture_diffuse1, v_TexCoords * u_TilingFactor) * u_Color;
}
