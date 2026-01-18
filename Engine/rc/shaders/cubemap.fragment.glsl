#version 460 core

in vec3 textureDir;

out vec4 FragColor;

uniform samplerCube u_CubeMapTexture;

void main()
{
    FragColor = texture(u_CubeMapTexture, textureDir);
}
