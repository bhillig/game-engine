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
	if (Core::Input::IsKeyPressed(KEY_W))
	{
		m_playerPosition.y += 1.0f * deltaTime;
	}
	else if (Core::Input::IsKeyPressed(KEY_S))
	{
		m_playerPosition.y += -1.0f * deltaTime;
	}
	if (Core::Input::IsKeyPressed(KEY_D))
	{
		m_playerPosition.x += 1.0f * deltaTime;
	}
	else if (Core::Input::IsKeyPressed(KEY_A))
	{
		m_playerPosition.x += -1.0f * deltaTime;
	}

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
