#pragma once

#include <EASTL/unique_ptr.h>
#include <EASTL/vector.h>
#include <SFML/Graphics/Transform.hpp>
#include "core/TransformTypes.hpp"

namespace sf
{
	class RenderWindow;
	class Drawable;
}

class Camera;
class PrimitiveComponent;
class HUD;

class Canvas
{
	eastl::unique_ptr<sf::RenderWindow> m_window;

	eastl::unique_ptr<Camera> m_camera;

	eastl::vector<PrimitiveComponent*> m_primitives;

	eastl::unique_ptr<HUD> m_hud;

	static eastl::unique_ptr<Canvas> m_instance;

public:

	static Canvas* getInstance() { return m_instance.get(); }

	void setWindow(sf::RenderWindow* window);

	void setHUD(HUD* hud);

	HUD* getHUD() const { return m_hud.get(); }

	sf::RenderWindow* getWindow() const { return m_window.get(); }

	void registry(PrimitiveComponent* primitive);

	void unregistry(PrimitiveComponent* primitive);

	void draw();

private:
	Canvas() = default;

	sf::Transform getToScreenTransform(const Transform& transform) const;
};