#pragma once

#include <Layer.h>

#include <OrthographicCameraController.h>

#include <Renderer/Texture.h>

namespace Core
{
	class MouseButtonPressedEvent;
}

namespace App
{

class Projectile
{
public:
	Projectile(const glm::vec2& position, const Core::Ref<Core::Texture>& texture)
		: m_position({ position.x, position.y, -0.2f })
		, m_texture(texture)
		, m_projectileSpeed(2.f)
		, m_lifeSpan(5.f)
		, m_isAlive(true)
	{
	}

	void Update(float deltaTime)
	{
		// Update position
		m_position.y += 2.f * deltaTime;

		// Update lifespan
		m_lifeSpan -= deltaTime;
		if (m_lifeSpan <= 0.f)
		{
			m_isAlive = false;
		}
	}
	
	const glm::vec3& GetPosition() const { return m_position; }
	void SetPosition(const glm::vec3& position) { m_position = position; }

	const Core::Ref<Core::Texture>& GetTexture() const { return m_texture; }

	bool IsAlive() const { return m_isAlive; }

private:
	glm::vec3 m_position;
	Core::Ref<Core::Texture> m_texture;
	float m_projectileSpeed;
	float m_lifeSpan;
	bool m_isAlive;
};

}

namespace App
{

class GameLayer : public Core::Layer
{
public:
	GameLayer();
	~GameLayer() override;

	void OnUpdate(float deltaTime) override;

	void OnImGuiRender() override;

	void OnEvent(Core::Event& event) override;

private:

	void SpawnProjectile();

	// Event callbacks
	bool OnMouseButtonPressed(Core::MouseButtonPressedEvent& event);

private:
	Core::OrthographicCameraController m_cameraController;

	Core::Ref<Core::Texture> m_playerSpaceShipTexture;
	Core::Ref<Core::Texture> m_playerProjectileTexture;
	Core::Ref<Core::Texture> m_starfieldTexture;

	glm::vec3 m_playerPosition {0, -0.6f, -0.1};

	std::vector<Projectile> m_projectiles;
};

}
