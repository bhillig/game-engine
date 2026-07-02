#include <Scene/Scene.h>

#include <Application.h>
#include <Renderer.h>

#include <Events/InputEvents.h>

#include <imgui.h>

#include <Renderer/RenderCommand.h>

#include "GLFW/glfw3.h"
#include "Renderer/BufferLayout.h"

using Core::AssetManager;
using Core::EventDispatcher;
using Core::Renderer;

namespace
{

	float Remap(float currentTime, float startTime, float endTime, float minValue, float maxValue)
	{
		const float fraction = (currentTime - startTime) / (endTime - startTime);
		return fraction * (maxValue - minValue) + minValue;
	}

	bool ClipLine(int dimension, const ECS::AABB& aabb, const glm::vec3& start, const glm::vec3& end, float& low, float& high)
	{
		float dimension_low = (aabb.min.operator[](dimension) - start.operator[](dimension)) / (end.operator[](dimension) - start.operator[](dimension));
		float dimension_high = (aabb.max.operator[](dimension) - start.operator[](dimension)) / (end.operator[](dimension) - start.operator[](dimension));

		if (dimension_low > dimension_high)
			std::swap(dimension_low, dimension_high);

		if (dimension_low > high || dimension_high < low)
		{
			return false;
		}

		low = std::max(dimension_low, low);
		high = std::min(dimension_high, high);
		return low <= high;
	}

	bool LineAABBIntersection(const ECS::AABB& aabb, const glm::vec3& start, const glm::vec3& end, glm::vec3& intersection, float& fraction)
	{
		float low = 0;
		float high = 1;

		// Clip X
		if (!ClipLine(0, aabb, start, end, low, high))
		{
			return false;
		}
		// Clip Y
		if (!ClipLine(1, aabb, start, end, low, high))
		{
			return false;
		}
		// Clip Z
		if (!ClipLine(2, aabb, start, end, low, high))
		{
			return false;
		}
		const glm::vec3 startToEnd = end - start;

		intersection = start + (startToEnd * low);
		fraction = low;
		return true;
	}

	bool PerformLineTraceIntersection(Scene& scene, const glm::vec3& start, const glm::vec3& end, HitResult& hitResult)
	{
		auto& entityManager = scene.GetEntityManager();

		// TODO: Remove AABB from ECS 
		std::vector<ECS::AABB> worldSpaceBoundingBoxes;

		// Iterate through all AABBs
		const size_t entityCount = entityManager.GetEntityCount();

		for (size_t i = 0; i < entityCount; ++i)
		{
			auto& entity = entityManager.GetEntity(i);
			if (!entity.HasComponent<ECS::BoundingBoxComponent>())
			{
				continue;
			}

			ECS::TransformComponent& transformComp = entity.GetComponent<ECS::TransformComponent>();
			ECS::BoundingBoxComponent& localBBoxComp = entity.GetComponent<ECS::BoundingBoxComponent>();

			// Transform the AABBS into world space
			const glm::vec3 worldMin = transformComp.transformMatrix() * glm::vec4(localBBoxComp.GetMin(), 1.0f);
			const glm::vec3 worldMax = transformComp.transformMatrix() * glm::vec4(localBBoxComp.GetMax(), 1.0f);
			worldSpaceBoundingBoxes.emplace_back(worldMin, worldMax);
		}

		float closestFraction = 1.f;
		glm::vec3 closestIntersection{};

		// For each bounding box, see if we get a hit
		for (const ECS::AABB& aabb : worldSpaceBoundingBoxes)
		{
			glm::vec3 intersection;
			float fraction;
			if (!LineAABBIntersection(aabb, start, end, intersection, fraction))
			{
				continue;
			}

			if (fraction < closestFraction)
			{
				closestFraction = fraction;
				closestIntersection = intersection;
			}
		}

		if (std::abs(closestFraction - 1.0f) >= 0.001f)
		{
			hitResult.Hit = true;
			hitResult.Intersection = closestIntersection;
			return true;
		}

		hitResult.Hit = false;
		return false;
	}

}

// Models
static constexpr std::string_view kBackpackModel = MODEL_DIR "/backpack/backpack.obj";
static constexpr std::string_view kGirlModel = MODEL_DIR "/girl/girl.obj";

// Shaders
static constexpr std::string_view kPositionAndTextureVS = SHADER_DIR "/positionAndTexCoords.vertex.glsl";
static constexpr std::string_view kColorFromTextureFS = SHADER_DIR "/colorFromTexture.fragment.glsl";

// Textures
static constexpr std::string_view kMartyPNG = TEXTURE_DIR "/MartySupremeMovieLogo.png";

Scene::Scene()
	: m_perspectiveCamera(glm::vec3{0.f, 0.f, -4.f}, 0.f, 90.f, 0.f, 45.f)
	, m_perspectiveCameraController(m_perspectiveCamera)
	, m_skyBoxEnabled(true)
{
	// Import all models 
	AssetManager::RequestLoadModel(std::string(kBackpackModel));
	AssetManager::RequestLoadModel(std::string(kGirlModel));

	auto posTexShader = Core::Renderer::GetShaderLibrary().Load("PosTexShader", kPositionAndTextureVS, kColorFromTextureFS);

	m_texture = Core::Texture::Create(std::string(kMartyPNG));

	posTexShader->Bind();
	posTexShader->SetUniform1i("u_Material.texture_diffuse1", 0);
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
	m_perspectiveCameraController.OnLoseControl();
}

void Scene::Update(float deltaTime)
{
	if (m_timeEnabled)
	{
		// Print current value
		const float minValue = 200.f;
		const float maxValue = 700.f;
		const float currentTime = glfwGetTime();
		const float currentValue = Remap(currentTime, m_timeStarted, m_timeFinished, minValue, maxValue);
		LOG_INFO("Value: [200 -> {} -> 700]", currentValue);
		if (currentTime > m_timeFinished)
		{
			m_timeEnabled = false;
		}
	}

	m_perspectiveCameraController.Update(deltaTime);

	Core::RenderCommand::SetClearColor({ 0.f, 0.f, 0.f, 1.f });
	Core::RenderCommand::Clear();

	// Retrieve aspect ratio
	const Core::Window& window = Core::Application::GetApp()->GetWindow();
	const float aspectRatio = static_cast<float>(window.GetWidth()) / static_cast<float>(window.GetHeight());

	// Gather scene data
	Renderer::SceneData sceneData;
	sceneData.ViewMatrix = m_perspectiveCamera.viewMatrix();
	sceneData.ProjectionMatrix = m_perspectiveCamera.projectionMatrix(aspectRatio);

	// Begin scene
	Renderer::BeginScene(sceneData);

	if (m_skyBoxEnabled)
	{
		Renderer::DrawSkybox();
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
			Renderer::DrawBoundingBox(transformComp.transformMatrix(), bbComp.GetMin(), bbComp.GetMax());
		}

	}

	// If we hit something, draw the line
	if (m_rayCastHit)
	{
		Renderer::DrawLine(m_rayCastOrigin, m_rayCastImpactPoint, glm::vec3(0, 1, 0));
	}

	// Draw Billboard
	{
		const glm::vec3 position(0.0, 5.0, 0.0);
		const glm::vec2 size(1, 1);
		const glm::vec4 color(1.0, 1.0, 0.0, 1.0);
		Renderer::DrawBillboard(position, size, color);
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
	m_perspectiveCameraController.OnKeyPressed(key);

	if (key == KEY_F)
	{
		const glm::vec3 start = m_perspectiveCamera.position();
		const glm::vec3 end = start + m_perspectiveCamera.forward() * 10000.f;

		HitResult hitResult;
		if (PerformLineTraceIntersection(*this, start, end, hitResult))
		{
			LOG_INFO("Hit object at intersection: ({},{},{})", hitResult.Intersection.x, hitResult.Intersection.y, hitResult.Intersection.z);

			m_rayCastHit = true;
			m_rayCastOrigin = start;
			m_rayCastImpactPoint = hitResult.Intersection;
		}
		else
		{
			LOG_INFO("No intersection...");
		}
	}
	else if (key == KEY_T)
	{
		if (!m_timeEnabled)
		{
			m_timeEnabled = true;
			m_timeStarted = static_cast<float>(glfwGetTime());
			m_timeFinished = m_timeStarted + 5.f;
		}
	}
	return false;
}

bool Scene::OnKeyReleased(int key)
{
	m_perspectiveCameraController.OnKeyReleased(key);
	return false;
}

bool Scene::OnMouseMove(double xPos, double yPos)
{
	m_perspectiveCameraController.OnMouseMove(xPos, yPos);
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
						if (ImGui::DragFloat3("Position", objectPos, 0.125f))
						{
							transformComp.position = glm::vec3(objectPos[0], objectPos[1], objectPos[2]);
						}

						float objectRot[3]{ transformComp.rotation.x, transformComp.rotation.y, transformComp.rotation.z };
						if (ImGui::DragFloat3("Rotation", objectRot, 0.125f))
						{
							transformComp.rotation = glm::vec3(objectRot[0], objectRot[1], objectRot[2]);
						}

						float objectScale[3]{ transformComp.scale.x, transformComp.scale.y, transformComp.scale.z };
						if (ImGui::DragFloat3("Scale", objectScale, 0.125f))
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

		ImGui::EndTabItem();
	}
}
