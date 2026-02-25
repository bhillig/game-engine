#include <Scene/Scene.h>

#include <Application.h>
#include <Renderer.h>

#include <Events/InputEvents.h>

#include <imgui.h>

#include <Renderer/RenderCommand.h>

#include "Renderer/BufferLayout.h"

using Core::AssetManager;
using Core::EventDispatcher;
using Core::Renderer;

// Models
static constexpr std::string_view kBackpackModel = MODEL_DIR "/backpack/backpack.obj";
static constexpr std::string_view kGirlModel = MODEL_DIR "/girl/girl.obj";

// Shaders
static constexpr std::string_view kPositionAndTextureVS = SHADER_DIR "/positionAndTexCoords.vertex.glsl";
static constexpr std::string_view kColorFromTextureFS = SHADER_DIR "/colorFromTexture.fragment.glsl";

// Textures
static constexpr std::string_view kMartyPNG = TEXTURE_DIR "/MartySupremeMovieLogo.png";

Scene::Scene()
	: m_orthoCameraController(1280.f / 720.f)
	, m_perspectiveCamera(glm::vec3{0.f, 0.f, -4.f}, 0.f, -90.f, 0.f, 45.f)
	, m_perspectiveCameraController(m_perspectiveCamera)
	, m_enable3DView(true)
	, m_skyBoxEnabled(true)
	, m_useSpaceSkybox(false)
{
	// Import all models 
	AssetManager::RequestLoadModel(std::string(kBackpackModel));
	AssetManager::RequestLoadModel(std::string(kGirlModel));

	auto posTexShader = Core::Renderer::GetShaderLibrary().Load("PosTexShader", kPositionAndTextureVS, kColorFromTextureFS);

	m_texture = Core::Texture::Create(std::string(kMartyPNG), 1);

	posTexShader->Bind();
	posTexShader->SetUniform1i("u_Material.texture_diffuse1", 1);
}

void Scene::OnEvent(Core::Event& event)
{
	if (!m_enable3DView)
	{
		m_orthoCameraController.OnEvent(event);
	}

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Core::MouseMovedEvent>([this](Core::MouseMovedEvent& event) { return OnMouseMove(event.GetX(), event.GetY()); });
	dispatcher.Dispatch<Core::KeyPressedEvent>([this](Core::KeyPressedEvent& event) { return OnKeyPressed(event.GetKeyCode()); });
	dispatcher.Dispatch<Core::KeyReleasedEvent>([this](Core::KeyReleasedEvent& event) { return OnKeyReleased(event.GetKeyCode()); });
}

void Scene::OnGainFocus()
{
}

void Scene::OnLoseFocus()
{
	if (m_enable3DView)
	{
		m_perspectiveCameraController.OnLoseControl();
	}
}

void Scene::Update(float deltaTime)
{
	if (m_enable3DView)
	{
		m_perspectiveCameraController.Update(deltaTime);
	}
	else
	{
		m_orthoCameraController.Update(deltaTime);
	}

	Core::RenderCommand::SetClearColor({ 0.f, 0.f, 0.f, 1.f });
	Core::RenderCommand::Clear();

	// Retrieve aspect ratio
	const Core::Window& window = Core::Application::GetApp()->GetWindow();
	const float aspectRatio = static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());

	// Gather scene data
	Renderer::SceneData sceneData;

	if (m_enable3DView)
	{
		sceneData.ViewMatrix = m_perspectiveCamera.viewMatrix();
		sceneData.ProjectionMatrix = m_perspectiveCamera.projectionMatrix(aspectRatio);
	}
	else
	{
		sceneData.ViewMatrix = m_orthoCameraController.viewMatrix();
		sceneData.ProjectionMatrix = m_orthoCameraController.projectionMatrix();
	}

	// Begin scene
	Renderer::BeginScene(sceneData);

	if (m_skyBoxEnabled)
	{
		if (m_useSpaceSkybox)
		{
			Renderer::DrawSpaceSkybox();

		}
		else
		{
			Renderer::DrawSkybox();
		}
	}

	for (size_t i = 0; i < m_entityManager.GetEntityCount(); ++i)
	{
		ECS::TransformComponent& transformComp = m_entityManager.GetComponentFromEntity<ECS::TransformComponent>(i);
		if (!transformComp.IsActive())
		{
			continue;
		}

		// Draw model
		ECS::MeshComponent& meshComp = m_entityManager.GetComponentFromEntity<ECS::MeshComponent>(i);
		if (meshComp.IsActive())
		{
			if (Core::Model* model = meshComp.GetModel())
			{
				Renderer::Submit(*model, transformComp.transformMatrix());
			}
		}

		// Draw bounding box
		ECS::BoundingBoxComponent& bbComp = m_entityManager.GetComponentFromEntity<ECS::BoundingBoxComponent>(i);
		if (bbComp.IsActive())
		{
			Renderer::DrawBoundingBox(bbComp.GetMin(), bbComp.GetMax());
		}

	}

	// Draw texture
	{
		auto va = Core::VertexArray::Create();

		// Create vertex array
		float vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.f, 1.0f };

		unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

		Core::BufferLayout layout{
			{
				{Core::ShaderDataType::Float3, "a_Pos"},
				{Core::ShaderDataType::Float2, "a_TexCoord"}
			}
		};

		auto vbo = Core::VertexBuffer::Create(vertices, sizeof(vertices));
		vbo->SetLayout(layout);
		va->AddVertexBuffer(vbo);

		auto ebo = Core::ElementBuffer::Create(indices, sizeof(indices));
		va->SetElementBuffer(ebo);

		m_texture->Bind();

		auto posTexShader = Core::Renderer::GetShaderLibrary().Get("PosTexShader");
		Renderer::Submit(posTexShader, va);
	}

	Renderer::EndScene();
}

void Scene::ImGuiRender()
{
	ImGui::Begin("Scene");
	if (ImGui::BeginTabBar("SceneTabs"))
	{
		ConstructLevelTreeTab();
		ConstructWorldTab();
		ConstructCameraTab();
		ImGui::EndTabBar();
	}
	ImGui::End();
}

bool Scene::OnKeyPressed(int key)
{
	if (m_enable3DView)
	{
		m_perspectiveCameraController.OnKeyPressed(key);
	}
	return false;
}

bool Scene::OnKeyReleased(int key)
{
	if (m_enable3DView)
	{
		m_perspectiveCameraController.OnKeyReleased(key);
	}
	return false;
}

bool Scene::OnMouseMove(double xPos, double yPos)
{
	if (m_enable3DView)
	{
		m_perspectiveCameraController.OnMouseMove(xPos, yPos);
	}
	return false;
}

void Scene::ConstructLevelTreeTab()
{
	if (ImGui::BeginTabItem("Level Tree"))
	{
		if (m_entityManager.GetEntityCount() == 0)
		{
			const ImVec4 grayTextColor(0.5f, 0.5f, 0.5f, 1.f);
			ImGui::TextColored(grayTextColor, "Right-click to add a game object");
		}

		if (ImGui::BeginPopupContextWindow("level_tree_context"))
		{
			if (ImGui::MenuItem("New Game Object"))
			{
				// Add a game object with a transform
				m_entityManager.CreateEntity().AddComponent<ECS::TransformComponent>();
			}
			ImGui::EndPopup();
		}

		for (size_t i = 0; i < m_entityManager.GetEntityCount(); ++i)
		{
			ECS::Entity& entity = m_entityManager.GetEntity(i);

			// Push unique ID for this entity
			ImGui::PushID(entity.GetID());

			const bool isNodeOpen = ImGui::TreeNode("gameObject", "Game Object %zu", entity.GetID());

			if (ImGui::BeginPopupContextItem())
			{
				const bool canAddComponents = !entity.HasComponent<ECS::MeshComponent>() || !entity.HasComponent<ECS::BoundingBoxComponent>();
				if (canAddComponents)
				{
					if (ImGui::BeginMenu("Add Component"))
					{
						if (!entity.HasComponent<ECS::MeshComponent>())
						{
							if (ImGui::MenuItem("Mesh"))
							{
								entity.AddComponent<ECS::MeshComponent>();
							}
						}

						if (!entity.HasComponent<ECS::BoundingBoxComponent>())
						{
							if (ImGui::MenuItem("Bounding Box"))
							{
								entity.AddComponent<ECS::BoundingBoxComponent>().SetSize(glm::vec3(1.f));
							}
						}

						ImGui::EndMenu();
					}
				}

				const bool canRemoveComponents = entity.HasComponent<ECS::MeshComponent>() || entity.HasComponent<ECS::BoundingBoxComponent>();
				if (canRemoveComponents)
				{
					if (ImGui::BeginMenu("Remove Component"))
					{
						if (entity.HasComponent<ECS::MeshComponent>())
						{
							if (ImGui::MenuItem("Mesh"))
							{
								entity.RemoveComponent<ECS::MeshComponent>();
							}
						}

						if (entity.HasComponent<ECS::BoundingBoxComponent>())
						{
							if (ImGui::MenuItem("Bounding Box"))
							{
								entity.RemoveComponent<ECS::BoundingBoxComponent>();
							}
						}

						ImGui::EndMenu();
					}
				}

				ImGui::EndPopup();
			}

			// Display TreeNode for Entity
			if (isNodeOpen)
			{
				// Display Entity Transform
				if (entity.HasComponent<ECS::TransformComponent>())
				{
					if (ImGui::CollapsingHeader("Transform"))
					{
						ECS::TransformComponent& transformComp = entity.GetComponent<ECS::TransformComponent>();

						float objectPos[3]{ transformComp.position.x, transformComp.position.y, transformComp.position.z };
						if (ImGui::DragFloat3("Position", objectPos))
						{
							transformComp.position = glm::vec3(objectPos[0], objectPos[1], objectPos[2]);
						}

						float objectRot[3]{ transformComp.rotation.x, transformComp.rotation.y, transformComp.rotation.z };
						if (ImGui::DragFloat3("Rotation", objectRot))
						{
							transformComp.rotation = glm::vec3(objectRot[0], objectRot[1], objectRot[2]);
						}

						float objectScale[3]{ transformComp.scale.x, transformComp.scale.y, transformComp.scale.z };
						if (ImGui::DragFloat3("Scale", objectScale))
						{
							transformComp.scale = glm::vec3(objectScale[0], objectScale[1], objectScale[2]);
						}
					}
				}

				ImGui::Spacing();

				// Display Entity Mesh
				if (entity.HasComponent<ECS::MeshComponent>())
				{
					if (ImGui::CollapsingHeader("Mesh"))
					{
						ECS::MeshComponent& meshComp = entity.GetComponent<ECS::MeshComponent>();

						// Get current model name for preview
						const std::string currentModelStr = meshComp.GetModel() ? meshComp.GetModel()->GetName() : "None";

						// List all possible models
						if (ImGui::BeginCombo("Model", currentModelStr.c_str()))
						{
							const bool isNoModelSelected = meshComp.GetModel() == nullptr;

							if (ImGui::Selectable("None", isNoModelSelected))
							{
								meshComp.SetModel(nullptr);
							}

							if (isNoModelSelected)
							{
								ImGui::SetItemDefaultFocus();
							}

							for (const auto& model : AssetManager::GetModels())
							{
								const bool isSelected = meshComp.GetModel() == model.get();

								if (ImGui::Selectable(model->GetName().c_str(), isSelected))
								{
									meshComp.SetModel(model.get());
								}

								if (isSelected)
								{
									ImGui::SetItemDefaultFocus();
								}
							}

							ImGui::EndCombo();
						}
					}
				}

				ImGui::Spacing();

				// Display Entity Bounding Box
				if (entity.HasComponent<ECS::BoundingBoxComponent>())
				{
					if (ImGui::CollapsingHeader("Bounding Box"))
					{
						ECS::BoundingBoxComponent& bbComp = entity.GetComponent<ECS::BoundingBoxComponent>();

						// Set Bounding Box Center
						const glm::vec3 center = bbComp.GetCenter();
						float bbPos[3]{ center.x, center.y, center.z };
						if (ImGui::DragFloat3("Center", bbPos, 0.25f))
						{
							bbComp.SetCenter(glm::vec3(bbPos[0], bbPos[1], bbPos[2]));
						}

						// Set Bounding Box Size
						const glm::vec3 size = bbComp.GetSize();
						float bbSize[3]{ size.x, size.y, size.z };
						if (ImGui::DragFloat3("Size", bbSize, 0.25f))
						{
							bbComp.SetSize(glm::vec3(bbSize[0], bbSize[1], bbSize[2]));
						}
					}
				}

				ImGui::TreePop();
			}

			// Pop unique ID for this entity
			ImGui::PopID();
		}

		ImGui::EndTabItem();
	}

}

void Scene::ConstructWorldTab()
{
	if (ImGui::BeginTabItem("World"))
	{
		bool skyBoxEnabled = m_skyBoxEnabled;
		if (ImGui::Checkbox("Enable Skybox", &skyBoxEnabled))
		{
			m_skyBoxEnabled = skyBoxEnabled;
		}

		bool useSpaceSkybox = m_useSpaceSkybox;
		if (ImGui::Checkbox("Use Space Skybox", &useSpaceSkybox))
		{
			m_useSpaceSkybox = useSpaceSkybox;
		}

		ImGui::EndTabItem();
	}
}

void Scene::ConstructCameraTab()
{
	if (ImGui::BeginTabItem("Camera"))
	{
		/*float cameraFOV = m_camera.fov();
		if (ImGui::SliderFloat("FOV", &cameraFOV, 1.f, 120.f))
		{
			m_camera.SetFOV(cameraFOV);
		}*/

		ImGui::Checkbox("Enable 3D", &m_enable3DView);
		ImGui::EndTabItem();
	}
}
