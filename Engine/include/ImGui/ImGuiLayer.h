#pragma once

#include <Layer.h>

namespace Core
{

class ImGuiLayer : public Layer
{
public:
	ImGuiLayer() = default;
	~ImGuiLayer() override;

	void OnAttach() override;

	void OnUpdate(float deltaTime) override;

	void OnRender() override;

};

}
