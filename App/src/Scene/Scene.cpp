#include <Scene/Scene.h>

#include <Application.h>
#include <Events/InputEvents.h>
#include <Renderer/Renderer.h>

#include <imgui.h>
#include <ranges>


using Core::EventDispatcher;

// TODO: Remove this hard-coded dependency once a more robust model system is implemented

// Vertex Shaders
const std::string kPositionNormalAndTexCoordVS = SHADER_DIR + std::string("/positionNormalAndTexCoords.vertex.glsl");

// Fragment Shaders
const std::string kColorFromTextureFS = SHADER_DIR + std::string("/colorFromTexture.fragment.glsl");

// Models
const std::string kBackpackModel = MODEL_DIR + std::string("/backpack/backpack.obj");
const std::string kGirlModel = MODEL_DIR + std::string("/girl/girl.obj");

Scene::Scene()
	: m_camera(nullptr), m_cameraController(nullptr)
{
	// Import all models 
	Core::Application::GetApp()->GetAssetManager().ImportModel(kBackpackModel);
	Core::Application::GetApp()->GetAssetManager().ImportModel(kGirlModel);

	// Create shaders
	m_modelShader = std::make_unique<Shader>(kPositionNormalAndTexCoordVS, kColorFromTextureFS);

	// Init camera
	const glm::vec3 cameraPos(0.0f, 0.0f, 4.0f);
	const float cameraFOV = 45.f;

	m_camera = std::make_unique<Camera>(cameraPos, 0.0f, -90.f, 0.0f, cameraFOV);
	m_cameraController = std::make_unique<CameraController>(*m_camera);

	// Set initial color
	const glm::vec4 bgColor(0.0f, 0.0f, 0.0f, 1.0f);
	Renderer::ClearColor(bgColor);
}

void Scene::Simulate(float deltaTime, unsigned int timeSteps /* = 1*/)
{
	for (unsigned int i = 0; i < timeSteps; ++i)
	{
		Update(deltaTime);
	}
	Render();
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
	Renderer::ClearColor(m_sceneColor);
	Renderer::ClearScreen();

	// View
	glm::mat4 view = m_camera->viewMatrix();
	m_modelShader->SetUniformMatrix4fv("u_View", glm::value_ptr(view));

	// Projection
	glm::mat4 projection = glm::perspective(glm::radians(m_camera->fov()), static_cast<float>(Core::Application::GetApp()->GetWindow().GetWidth()) / Core::Application::GetApp()->GetWindow().GetHeight(), 0.1f, 100.f);
	m_modelShader->SetUniformMatrix4fv("u_Projection", glm::value_ptr(projection));

	for (size_t i = 0; i < m_entityManager.GetEntityCount(); ++i)
	{
		ECS::TransformComponent& transformComp = m_entityManager.GetComponentFromEntity<ECS::TransformComponent>(i);
		ECS::MeshComponent& meshComp = m_entityManager.GetComponentFromEntity<ECS::MeshComponent>(i);
		if (!transformComp.IsActive() || !meshComp.IsActive())
		{
			continue;
		}

		// Set model matrix for model
		glm::mat4 modelObj(1.f);
		modelObj = glm::translate(modelObj, glm::vec3(transformComp.x, transformComp.y, transformComp.z));
		m_modelShader->SetUniformMatrix4fv("u_Model", glm::value_ptr(modelObj));

		// Draw model
		if (Model* model = meshComp.GetModel())
		{
			model->Draw(*m_modelShader);
		}
	}

	ConstructGUI();
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

void Scene::ConstructGUI()
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

void Scene::ConstructLevelTreeTab()
{
	if (ImGui::BeginTabItem("Level Tree"))
	{
		// Retrieve AssetManager
		Core::AssetManager& assetManager = Core::Application::GetApp()->GetAssetManager();

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
				const bool canAddComponents = !entity.HasComponent<ECS::MeshComponent>();
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

						ImGui::EndMenu();
					}
				}

				const bool canRemoveComponents = entity.HasComponent<ECS::MeshComponent>();
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

						float objectPos[3]{ static_cast<float>(transformComp.x), static_cast<float>(transformComp.y), static_cast<float>(transformComp.z) };
						if (ImGui::DragFloat3("Position", objectPos))
						{
							transformComp.x = static_cast<double>(objectPos[0]);
							transformComp.y = static_cast<double>(objectPos[1]);
							transformComp.z = static_cast<double>(objectPos[2]);
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

							for (const auto& model : std::views::values(assetManager.GetModels()))
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
		float sceneColor[4] = { m_sceneColor.r, m_sceneColor.g, m_sceneColor.b, m_sceneColor.a };
		if (ImGui::ColorPicker4("Scene Color", sceneColor))
		{
			m_sceneColor = glm::vec4(sceneColor[0], sceneColor[1], sceneColor[2], sceneColor[3]);
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
