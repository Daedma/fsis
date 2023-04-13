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
	if (m_camera && !m_primitives.empty())
	{
		Transform cameraTransform = m_camera->getProjectionMatrix();
		eastl::sort(m_primitives.begin(), m_primitives.end(),
			[&cameraTransform](PrimitiveComponent* lhs, PrimitiveComponent* rhs) {
				Vector3f z(0.f, 0.f, 1.f);
				return (z * lhs->getWorldTransform() * cameraTransform).z <
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
	float maxDimension = eastl::max_alt(m_window->getSize().x, m_window->getSize().y);
	Transform projection = transform * Transform(mathter::Scale(maxDimension, maxDimension, 0.f));
	sf::Transform trans2d(
		projection(0, 0), projection(0, 1), projection(0, 3),
		projection(1, 0), projection(1, 1), projection(1, 3),
		projection(3, 0), projection(3, 1), projection(3, 3)
	);
	return trans2d;
}

