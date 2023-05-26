#pragma once

#include <EASTL/unique_ptr.h>
#include <EASTL/vector.h>
#include <EASTL/vector_map.h>
#include <EASTL/vector_set.h>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
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
	static eastl::unique_ptr<sf::RenderWindow> m_window;

	static eastl::unique_ptr<Camera> m_camera;

	static eastl::unique_ptr<HUD> m_hud;

	static eastl::vector_map<int32_t, eastl::vector<PrimitiveComponent*>> m_layers;

	static eastl::vector_map<int32_t, bool> m_stableLayers;

public:
	Canvas() = delete;

	static void init(const char* title);

	static sf::RenderWindow* getWindow() { return m_window.get(); }

	static HUD* getHUD() { return m_hud.get(); }

	template<typename T>
	static T* setHUD()
	{
		T* hud = new T(m_window.get());
		m_hud.reset(hud);
		return hud;
	}

	static Camera* getCamera() { return m_camera.get(); }

	static void resetCamera();

	template<typename T, typename ...Args>
	static T* createCamera(Args&&... args);

	static void addStableLayer(int32_t layer) { m_stableLayers[layer] = false; }

	static bool isStableLayer(int32_t layer) { return m_stableLayers.count(layer); }

	static void registry(PrimitiveComponent* primitive);

	static void unregistry(PrimitiveComponent* primitive);

	static void draw();

private:
	static sf::Transform getToScreenTransform(const Transform& transform);

	static void updateOrderZ(const Transform& projection);
};

template<typename T, typename ...Args>
T* Canvas::createCamera(Args&&... args)
{
	m_camera = eastl::make_unique<T>(eastl::forward<Args>(args)...);
	return dynamic_cast<T*>(m_camera.get());
}