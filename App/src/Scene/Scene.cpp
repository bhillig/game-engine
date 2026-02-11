#include <Scene/Scene.h>

#include <Application.h>
#include <Renderer.h>

#include <Events/InputEvents.h>

#include <imgui.h>

using Core::AssetManager;
using Core::EventDispatcher;
using Core::Renderer;

// Models
static constexpr std::string_view kBackpackModel = MODEL_DIR "/backpack/backpack.obj";
static constexpr std::string_view kGirlModel = MODEL_DIR "/girl/girl.obj";

Scene::Scene()
	: m_camera(nullptr), m_cameraController(nullptr), m_skyBoxEnabled(true), m_useSpaceSkybox(false)
{
	// Import all models 
	AssetManager::RequestLoadModel(std::string(kBackpackModel));
	AssetManager::RequestLoadModel(std::string(kGirlModel));

	// Init camera
	constexpr glm::vec3 cameraPos(0.0f, 0.0f, -4.0f);
	constexpr float cameraFOV = 45.f;

	m_camera = std::make_unique<Camera>(cameraPos, 0.0f, -90.f, 0.0f, cameraFOV);
	m_cameraController = std::make_unique<CameraController>(*m_camera);

	// Set initial color
	constexpr glm::vec4 bgColor(0.0f, 0.0f, 0.0f, 1.0f);
	Renderer::ClearColor(bgColor);
}

void Scene::Simulate(float deltaTime, unsigned int timeSteps /* = 1*/)
{
	for (unsigned int i = 0; i < timeSteps; ++i)
	{
		Update(deltaTime);
	}
}

void Scene::OnEvent(Core::Event& event)
{
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
	if (m_cameraController)
	{
		m_cameraController->OnLoseControl();
	}
}

void Scene::Update(float deltaTime)
{
	if (m_cameraController)
	{
		m_cameraController->Update(deltaTime);
	}
}

void Scene::Render()
{
	Renderer::ClearColor(glm::vec4(0, 0, 0, 1));
	Renderer::ClearScreen();

	// Retrieve aspect ratio
	const Core::Window& window = Core::Application::GetApp()->GetWindow();
	const float aspectRatio = static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());

	Renderer::BeginScene(m_camera->viewMatrix(), m_camera->projectionMatrix(aspectRatio));

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
			if (Model* model = meshComp.GetModel())
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
	if (m_cameraController)
	{
		m_cameraController->OnKeyPressed(key);
	}
	return false;
}

bool Scene::OnKeyReleased(int key)
{
	if (m_cameraController)
	{
		m_cameraController->OnKeyReleased(key);
	}
	return false;
}

bool Scene::OnMouseMove(double xPos, double yPos)
{
	if (m_cameraController)
	{
		m_cameraController->OnMouseMove(xPos, yPos);
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
		float cameraFOV = m_camera->fov();
		if (ImGui::SliderFloat("FOV", &cameraFOV, 1.f, 120.f))
		{
			m_camera->SetFOV(cameraFOV);
		}
		ImGui::EndTabItem();
	}
}
