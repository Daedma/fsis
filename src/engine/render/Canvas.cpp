#include "render/Canvas.hpp"
#include "render/Camera.hpp"
#include "gui/HUD.hpp"
#include "components/PrimitiveComponent.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <EASTL/sort.h>
#include <EASTL/vector.h>

eastl::unique_ptr<sf::RenderWindow> Canvas::m_window = nullptr;

eastl::unique_ptr<Camera> Canvas::m_camera = nullptr;

eastl::unique_ptr<HUD> Canvas::m_hud = nullptr;

eastl::vector_map<int32_t, eastl::vector<PrimitiveComponent*>> Canvas::m_layers;

eastl::vector_map<int32_t, bool> Canvas::m_stableLayers;

void Canvas::resetCamera()
{
	m_camera.reset();
}

void Canvas::init(const char* title)
{
	m_window.reset(new sf::RenderWindow(sf::VideoMode::getDesktopMode(), title, sf::Style::None));
	m_window->setKeyRepeatEnabled(false);
	m_hud.reset(new HUD(m_window.get()));
}

void Canvas::registry(PrimitiveComponent* primitive)
{
	auto& layer = m_layers[primitive->getLayer()];
	if (eastl::find(layer.cbegin(), layer.cend(), primitive)
		== layer.cend())
	{
		layer.emplace_back(primitive); // safe
		auto stable = m_stableLayers.find(primitive->getLayer());
		if (stable != m_stableLayers.end())
		{
			stable->second = false;
		}
	}
}

void Canvas::unregistry(PrimitiveComponent* primitive)
{
	auto& layer = m_layers[primitive->getLayer()];
	layer.erase_first(primitive);
}

void Canvas::draw()
{
	if (!m_window)
	{
		return;
	}
	m_window->clear(sf::Color::Cyan);
	if (m_camera && !m_layers.empty())
	{
		Transform cameraTransform = m_camera->getProjectionMatrix();
		updateOrderZ(cameraTransform);
		for (const auto& [n, layer] : m_layers)
		{
			for (PrimitiveComponent* i : layer)
			{
				i->draw(*m_window, getToScreenTransform(i->getWorldTransform() * cameraTransform));
			}
		}
	}
	if (m_hud)
	{
		m_hud->draw();
	}
	m_window->display();
}

sf::Transform Canvas::getToScreenTransform(const Transform& transform)
{
	float minDimension = eastl::min_alt(m_window->getSize().x, m_window->getSize().y);
	Transform projection = transform * Transform(mathter::Scale(minDimension, minDimension, minDimension));
	Vector3f translation = TSR::getTranslation(projection);
	Vector3f scale = TSR::getScale(projection);
	// We don't need rotation for 2d srites
	sf::Transform trans2d;
	// Center Camera
	trans2d.translate(translation.x + m_window->getSize().x / 2, translation.y + m_window->getSize().y / 2);
	trans2d.scale(scale.x, scale.y);
	return trans2d;
}

void Canvas::updateOrderZ(const Transform& projection)
{
	for (auto& [n, layer] : m_layers)
	{
		auto stable = m_stableLayers.find(n);
		if (stable != m_stableLayers.end())
		{
			if (stable->second)
			{
				continue;
			}
			else
			{
				stable->second = true;
			}
		}
		eastl::stable_sort(layer.begin(), layer.end(),
			[&projection](PrimitiveComponent* lhs, PrimitiveComponent* rhs) {
				return (Z_AXIS * lhs->getWorldTransform() * projection).z >
					(Z_AXIS * rhs->getWorldTransform() * projection).z;
			});
	}
}