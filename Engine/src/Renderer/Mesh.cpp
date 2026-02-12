#include <Renderer/Mesh.h>

#include <glad/glad.h>

#include <Renderer/VertexBufferLayout.h>

namespace Core
{

Mesh::Mesh(MeshData&& meshData)
	: m_vertices(std::move(meshData.vertices))
	, m_indices(std::move(meshData.indices))
	, m_textures(std::move(meshData.textureReferences))
{
	m_vbo = std::unique_ptr<VertexBuffer>(VertexBuffer::Create(m_vertices.data(), m_vertices.size() * sizeof(Vertex)));
	m_ebo = std::unique_ptr<ElementBuffer>(ElementBuffer::Create(m_indices.data(), m_indices.size() * sizeof(unsigned int)));
	m_vao.Bind();
	m_vbo->Bind();
	m_ebo->Bind();

	// Specify how our vertex data is formatted
	VertexBufferLayout vertexBufferLayout;
	vertexBufferLayout.Push<float>(3); // Position
	vertexBufferLayout.Push<float>(3); // Normal
	vertexBufferLayout.Push<float>(2); // Texture Coordinates
	m_vao.Add(*m_vbo, vertexBufferLayout);

	m_vao.Unbind();
	m_vbo->Unbind();
	m_ebo->Unbind();
}

// Move constructor
Mesh::Mesh(Mesh&& other) noexcept
	: m_vertices(std::move(other.m_vertices))
	, m_indices(std::move(other.m_indices))
	, m_textures(std::move(other.m_textures))
	, m_vao(std::move(other.m_vao))
	, m_ebo(std::move(other.m_ebo))// important
{}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
	if (this != &other)
	{
		m_vertices = std::move(other.m_vertices);
		m_indices = std::move(other.m_indices);
		m_textures = std::move(other.m_textures);
		m_vao = std::move(other.m_vao);
	}
	return *this;
}

void Mesh::Draw(Shader& shader) const
{
	shader.Bind();

	unsigned int diffuseCount = 1;
	unsigned int specularCount = 1;
	for (unsigned int i = 0; i < m_textures.size(); ++i)
	{
		std::string number;
		std::string type = m_textures[i].type;
		if (type == "texture_diffuse")
		{
			number = std::to_string(diffuseCount++);
		}
		else if (type == "texture_specular")
		{
			number = std::to_string(specularCount++);
		}

		const std::string textureUniformName = std::string("u_Material.") + type + number;
		if (shader.SetUniform1i(textureUniformName.c_str(), i))
		{
			m_textures[i].texture->Bind(i);
		}
	}

	// Draw mesh
	m_vao.Bind();
	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

	shader.Unbind();
	m_vao.Unbind();
}

}