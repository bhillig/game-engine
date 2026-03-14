#version 410 core

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

in vec2 v_TexCoords;

out vec4 FragColor;

uniform Material u_Material;

void main()
{
    FragColor = texture(u_Material.texture_diffuse1, v_TexCoords);
}
