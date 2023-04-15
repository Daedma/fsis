#include "render/Canvas.hpp"
#include "render/Camera.hpp"
#include "gui/HUD.hpp"
#include "components/PrimitiveComponent.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <EASTL/sort.h>
#include <EASTL/vector.h>

eastl::unique_ptr<Canvas> Canvas::m_instance{new Canvas()};

Canvas::Canvas() : m_window(nullptr), m_hud(nullptr), m_camera(new Camera())
{
	m_camera->setView(Camera::ISOMETRIC);
}

Canvas::~Canvas() {}

void Canvas::setWindow(sf::RenderWindow* window)
{
	m_window.reset(window);
	m_window->setKeyRepeatEnabled(false);
}

void Canvas::setHUD(HUD* hud)
{
	m_hud.reset(hud);
}

void Canvas::setCamera(Camera* camera) { m_camera.reset(camera); }

void Canvas::registry(PrimitiveComponent* primitive)
{
	if (eastl::find(m_primitives.cbegin(), m_primitives.cend(), primitive)
		== m_primitives.cend())
	{
		m_primitives.emplace_back(primitive);
	}
}

void Canvas::unregistry(PrimitiveComponent* primitive)
{
	m_primitives.erase_first(primitive);
}

void Canvas::draw()
{
	if (!m_window)
	{
		return;
	}
	m_window->clear();
	if (m_camera && !m_primitives.empty())
	{
		Transform cameraTransform = m_camera->getProjectionMatrix();
		eastl::sort(m_primitives.begin(), m_primitives.end(),
			[&cameraTransform](PrimitiveComponent* lhs, PrimitiveComponent* rhs) {
				Vector3f z(0.f, 0.f, 1.f);
				return (z * lhs->getWorldTransform() * cameraTransform).z >
					(z * rhs->getWorldTransform() * cameraTransform).z;
			});
		for (PrimitiveComponent* i : m_primitives)
		{
			i->draw(*m_window, getToScreenTransform(i->getWorldTransform() * cameraTransform));
		}
	}
	if (m_hud)
	{
		m_hud->draw(*m_window, sf::RenderStates::Default);
	}
	m_window->display();
}

sf::Transform Canvas::getToScreenTransform(const Transform& transform) const
{
	float minDimension = eastl::min_alt(m_window->getSize().x, m_window->getSize().y);
	Transform projection = transform * Transform(mathter::Scale(minDimension, minDimension, 1.f));
	Vector3f translation = TSR::getTranslation(projection);
	Vector3f scale = TSR::getScale(projection);
	// We don't need rotation for 2d srites
	sf::Transform trans2d;
	// Center Camera
	trans2d.translate(translation.x + m_window->getSize().x / 2, translation.y + m_window->getSize().y / 2);
	trans2d.scale(scale.x, scale.y);
	return trans2d;
}

