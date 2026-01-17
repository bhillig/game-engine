#version 460 core

in vec2 textureCoord;

out vec4 FragColor;

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

uniform float u_MixAmount;

void main()
{
    FragColor = mix(texture(u_Texture1, textureCoord), texture(u_Texture2, textureCoord), u_MixAmount);
}
