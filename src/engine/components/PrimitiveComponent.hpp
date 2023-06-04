/**
 * @file PrimitiveComponent.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of PrimitiveComponent
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "components/SceneComponent.hpp"
#include <SFML/Graphics/Drawable.hpp>

/**
 * @brief Scene component that can be drawn
 */
class PrimitiveComponent : public SceneComponent, public sf::Drawable
{
public:
	PrimitiveComponent();

	PrimitiveComponent(Actor* owner);

	PrimitiveComponent(SceneComponent* parent);

	virtual ~PrimitiveComponent();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	int32_t getLayer() const { return m_layer; }

	void setLayer(int32_t layer);

	void hide();

	void show();

private:
	int32_t m_layer = 0;
};