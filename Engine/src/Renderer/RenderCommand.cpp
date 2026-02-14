#include <Renderer/RenderCommand.h>

#include <Platform/OpenGL/OpenGLRendererAPI.h>

namespace Core
{
	RendererAPI* RenderCommand::s_rendererAPI = new OpenGLRendererAPI();
}
