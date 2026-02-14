project(Engine)

# Set C++23 verison
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate OpenGL
find_package(OpenGL REQUIRED)

include(FetchContent)

# ----------------------------------------------------------
# GLFW
# ----------------------------------------------------------
FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

# ----------------------------------------------------------
# Glad
# ----------------------------------------------------------
add_subdirectory(external/glad)

# ----------------------------------------------------------
# stb_image
# ----------------------------------------------------------
add_subdirectory(external/stb_image)

# ----------------------------------------------------------
# Dear ImGui
# ----------------------------------------------------------
add_library(imgui
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC external/imgui external/imgui/backends)
target_link_libraries(imgui PUBLIC OpenGL::GL glfw)

# ----------------------------------------------------------
# assimp - Open-Asset-Importer-Library
# ----------------------------------------------------------
add_subdirectory(external/assimp)

# ----------------------------------------------------------
# spdlog - Fast C++ logging library
# ----------------------------------------------------------
add_subdirectory(external/spdlog)

set(SOURCES 
	src/Application.cpp
	src/AssetManager.cpp
	src/Camera.cpp
	src/CameraController.cpp
	src/Log.cpp
	src/Renderer.cpp
	src/ECS/EntityManager.cpp
	src/ECS/EntityMemoryPool.cpp
	src/ImGui/CoreImGui.cpp
	src/Platform/OpenGL/OpenGLElementBuffer.cpp
	src/Platform/OpenGL/OpenGLGraphicsContext.cpp
	src/Platform/OpenGL/OpenGLRendererAPI.cpp
	src/Platform/OpenGL/OpenGLVertexArray.cpp
	src/Platform/OpenGL/OpenGLVertexBuffer.cpp
	src/Platform/Windows/WindowsInput.cpp
	src/Platform/Windows/WindowsWindow.cpp
	src/Renderer/CubemapTexture.cpp
	src/Renderer/ElementBuffer.cpp
	src/Renderer/Mesh.cpp
	src/Renderer/Model.cpp
	src/Renderer/ModelLoader.cpp
	src/Renderer/RenderCommand.cpp
	src/Renderer/RendererAPI.cpp
	src/Renderer/Shader.cpp
	src/Renderer/Texture.cpp
	src/Renderer/VertexArray.cpp
	src/Renderer/VertexBuffer.cpp
	src/Utils/FileUtils.cpp)

set(HEADERS
	include/Application.h
	include/AssetManager.h
	include/Camera.h
	include/CameraController.h
	include/Input.h
	include/InputKeyCodes.h
	include/Layer.h
	include/Log.h
	include/Renderer.h
	include/Window.h
	include/ECS/Component.h
	include/ECS/Entity.h
	include/ECS/EntityManager.h
	include/ECS/EntityMemoryPool.h
	include/Events/Event.h
	include/Events/InputEvents.h
	include/Events/WindowEvents.h
	include/ImGui/CoreImGui.h
	include/Platform/OpenGL/OpenGLElementBuffer.h
	include/Platform/OpenGL/OpenGLGraphicsContext.h
	include/Platform/OpenGL/OpenGLRendererAPI.h
	include/Platform/OpenGL/OpenGLVertexArray.h
	include/Platform/OpenGL/OpenGLVertexBuffer.h
	include/Platform/Windows/WindowsInput.h
	include/Platform/Windows/WindowsWindow.h
	include/Renderer/BufferLayout.h
	include/Renderer/CubemapTexture.h
	include/Renderer/ElementBuffer.h
	include/Renderer/GraphicsContext.h
	include/Renderer/Mesh.h
	include/Renderer/Model.h
	include/Renderer/ModelLoader.h
	include/Renderer/RenderCommand.h
	include/Renderer/RendererAPI.h
	include/Renderer/Shader.h
	include/Renderer/Texture.h
	include/Renderer/VertexArray.h
	include/Renderer/VertexBuffer.h
	include/Utils/FileUtils.h
	include/Utils/ThreadSafeQueue.h)

add_library(Engine STATIC ${SOURCES} ${HEADERS})

target_link_libraries(Engine PUBLIC OpenGL::GL glfw glad stb_image imgui assimp spdlog)

target_include_directories(Engine PUBLIC include)

# Sync Visual Studio file directories to what's on disk
set(ALL_FILES ${SOURCES} ${HEADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${ALL_FILES})

# ----------------------------------------------------------
# GLM (Header Only Math Library)
# ----------------------------------------------------------
target_include_directories(Engine PUBLIC external/glm/include)

# Precompile Header
target_precompile_headers(Engine PUBLIC "include/gepch.h")

# Define SHADER_DIR
target_compile_definitions(Engine PRIVATE SHADER_DIR="$<TARGET_FILE_DIR:Engine>/rc/shaders")
# Define TEXTURE_DIR
target_compile_definitions(Engine PRIVATE TEXTURE_DIR="$<TARGET_FILE_DIR:Engine>/rc/textures")

# Copy resources to build folder
add_custom_command(TARGET Engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/rc"
        "$<TARGET_FILE_DIR:Engine>/rc"
)
