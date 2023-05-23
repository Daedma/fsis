#pragma once

#include <EASTL/unique_ptr.h>
#include <EASTL/vector.h>
#include <EASTL/vector_map.h>
#include <EASTL/vector_set.h>
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

	eastl::unique_ptr<HUD> m_hud;

	eastl::vector_map<int32_t, eastl::vector<PrimitiveComponent*>> m_layers;

	eastl::vector_map<int32_t, bool> m_stableLayers;

	static eastl::unique_ptr<Canvas> m_instance;

public:
	//TODO add stable layers
	~Canvas();

	// NOTE maybe we don't need singletone? We can use only static methods
	static Canvas* getInstance() { return m_instance.get(); }

	// TODO remove this
	// incapsulate window creation process
	void setWindow(sf::RenderWindow* window);

	sf::RenderWindow* getWindow() const { return m_window.get(); }

	// TODO make template version (no-new)
	void setHUD(HUD* hud);

	HUD* getHUD() const { return m_hud.get(); }

	// TODO incapsulate camera setup
	// make template version
	void setCamera(Camera* camera);

	Camera* getCamera() const { return m_camera.get(); }

	void addStableLayer(int32_t layer) { m_stableLayers[layer] = false; }

	bool isStableLayer(int32_t layer) { return m_stableLayers.count(layer); }

	void registry(PrimitiveComponent* primitive);

	void unregistry(PrimitiveComponent* primitive);

	void draw();

private:
	// If we refuse to singletone it whould be to declared as deleted 
	Canvas();

	sf::Transform getToScreenTransform(const Transform& transform) const;

	void updateOrderZ(const Transform& projection);
};