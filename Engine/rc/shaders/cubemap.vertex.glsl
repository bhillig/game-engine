#version 460 core

layout (location = 0) in vec3 aPos;

out vec3 textureDir;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    mat4 view = mat4(mat3(u_View));  // Strip translation of camera
    vec4 pos = u_Projection * view * vec4(aPos, 1);
    // Overwrite z to be w so when the depth test gets calculated (z/w) we guarantee the skybox to fail and prioritize other objects in front
    gl_Position = pos.xyww;
    textureDir = aPos;
}
