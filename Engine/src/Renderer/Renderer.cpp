#include <Renderer/Renderer.h>

#include <Renderer/VertexArray.h>
#include <Renderer/VertexBuffer.h>
#include <Renderer/VertexBufferLayout.h>

#include <glad/glad.h>

namespace Renderer
{

void ClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void ClearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DrawLine(const glm::vec3& a, const glm::vec3& b)
{
	float vertices[] = {
		a.x, a.y, a.z, 
		b.x, b.y, b.z
	};

	VertexBufferLayout layout;
	layout.Push<float>(3);

	VertexBuffer vbo(vertices, sizeof(vertices));

	VertexArray vao;
	vao.Add(vbo, layout);

	glDrawElements(GL_TRIANGLES, 2, GL_UNSIGNED_INT, 0);
}

}
