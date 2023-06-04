/**
 * @file Monster.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of Monster class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "FSISCharacter.hpp"
#include "components/AudioComponent.hpp"

/**
 * @brief A class represents monster on arena in FSIS
 */
class Monster : public FSISCharacter
{
public:
	Monster(World* world);

	virtual void setupController(Controller* controller) override;

	virtual void attack() override;

	virtual void takeDamage(float value, FSISCharacter* instigator) override;

protected:
	virtual void load_Internal(const boost::json::object& obj) override;

	virtual void onBeginTargeted(FSISCharacter* hunter) override;

	virtual void onEndTargeted(FSISCharacter* hunter) override;

	virtual void onDeath(FSISCharacter* killer) override;

private:
	class SpriteComponent* m_marker = nullptr;

	AudioComponent* m_sound = nullptr;
};