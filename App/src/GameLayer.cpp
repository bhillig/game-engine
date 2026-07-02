#include <GameLayer.h>

#include <Renderer.h>

#include <Renderer/RenderCommand.h>
#include <Renderer/Renderer2D.h>

#include <imgui.h>

#include <Input.h>

#include "Events/InputEvents.h"

// Audio
static constexpr std::string_view kSpaceshipShootSound = AUDIO_DIR "/spaceship-shoot.wav";

// Textures
static constexpr std::string_view kSpaceshipTexture = TEXTURE_DIR "/spaceship.png";
static constexpr std::string_view kPlayerProjectileTexture = TEXTURE_DIR "/player-projectile.png";
static constexpr std::string_view kStarfieldTexture = TEXTURE_DIR "/starfield.png";

namespace
{

glm::vec2 Approach(const glm::vec2& target, const glm::vec2& currentValue, float deltaTime, float rate = 1.f)
{
	const glm::vec2 diff = target - currentValue;
	const float dist = glm::length(diff);
	const float step = deltaTime * rate;

	if (dist > step)
	{
		const glm::vec2 direction = glm::normalize(diff);
		return currentValue + direction * step;
	}

	return target;
}

}


namespace App
{

GameLayer::GameLayer()
	: m_cameraController(1280.f / 720.f)
{

	// TEMPORARY: Initialize sound engine
	const ma_result result = ma_engine_init(nullptr, &m_soundEngine);
	if (result != ma_result::MA_SUCCESS) {
		LOG_CORE_ERROR("Failed to initialize sound engine!");
	}

	m_playerSpaceShipTexture = Core::Texture::Create(std::string(kSpaceshipTexture));
	m_playerProjectileTexture = Core::Texture::Create(std::string(kPlayerProjectileTexture));
	m_starfieldTexture = Core::Texture::Create(std::string(kStarfieldTexture));
}

GameLayer::~GameLayer()
{
	// TEMPORARY
	ma_engine_uninit(&m_soundEngine);
}

void GameLayer::OnUpdate(float deltaTime)
{
	// Player movement input
	glm::vec2 playerInput{ 0, 0 };

	if (Core::Input::IsKeyPressed(KEY_W))
	{
		playerInput.y = 1.f;
	}
	else if (Core::Input::IsKeyPressed(KEY_S))
	{
		playerInput.y = -1.f;
	}
	if (Core::Input::IsKeyPressed(KEY_D))
	{
		playerInput.x = 1.f;
	}
	else if (Core::Input::IsKeyPressed(KEY_A))
	{
		playerInput.x = -1.f;
	}

	if (glm::length(playerInput) > 0.f)
	{
		playerInput = glm::normalize(playerInput);
	}

	const float playerSpeed = 1.f;
	m_playerGoalVelocity = playerInput * playerSpeed;

	// Lerp to desired velocity
	m_playerVelocity = Approach(m_playerGoalVelocity, m_playerVelocity, deltaTime, 1.f);

	// Print out player velocity
	LOG_INFO("Player velocity = ({}, {})", m_playerVelocity.x, m_playerVelocity.y);

	// Update player position
	m_playerPosition.x += m_playerVelocity.x * deltaTime;
	m_playerPosition.y += m_playerVelocity.y * deltaTime;

	// Update projectiles
	for (auto& projectile : m_projectiles)
	{
		projectile.Update(deltaTime);
	}

	// Update camera
	m_cameraController.Update(deltaTime);

	// Render
	Core::RenderCommand::SetClearColor(glm::vec4(0.f, 0.2f, 0.3f, 1.0f));
	Core::RenderCommand::Clear();
	
	// Begin Scene
	Core::Renderer2D::BeginScene(m_cameraController.GetCamera());

	// Draw space background
	Core::Renderer2D::DrawQuad(glm::vec2{ 0.f, 0.f }, 0.f, glm::vec2{ 100.f, 100.f }, m_starfieldTexture, 80);

	// Draw player
	Core::Renderer2D::DrawQuad(m_playerPosition, 0.f, glm::vec2{ 0.15f, 0.15f }, m_playerSpaceShipTexture);

	// Draw projectiles
	for (const auto& projectile : m_projectiles)
	{
		Core::Renderer2D::DrawQuad(projectile.GetPosition(), 0.f, glm::vec2{ 0.2f, 0.2f }, projectile.GetTexture());
	}

	// End Scene
	Core::Renderer2D::EndScene();

	// Cleanup projectile
	std::erase_if(m_projectiles, [](const Projectile& projectile) { return !projectile.IsAlive(); });
}

void GameLayer::OnImGuiRender()
{
}

void GameLayer::OnEvent(Core::Event& event)
{
	Core::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Core::MouseButtonPressedEvent>(CORE_BIND_FN(GameLayer::OnMouseButtonPressed));

	m_cameraController.OnEvent(event);
}

void GameLayer::SpawnProjectile()
{
	// Spawn projectile object
	const glm::vec2 projectileSpawnLocation{ m_playerPosition.x, m_playerPosition.y + 0.15f };
	m_projectiles.emplace_back(Projectile{ projectileSpawnLocation, m_playerProjectileTexture });

	// Play sound effect
	ma_engine_play_sound(&m_soundEngine, kSpaceshipShootSound.data(), nullptr);
}

bool GameLayer::OnMouseButtonPressed(Core::MouseButtonPressedEvent& event)
{
	if (event.GetButton() == MOUSE_BUTTON_LEFT)
	{
		SpawnProjectile();
	}
	return false;
}
}
