#pragma once

#include "FSISCharacter.hpp"

class Monster : public FSISCharacter
{
public:
	Monster(World* world);

	virtual void setupController(Controller* controller) override;

	virtual void attack() override;

protected:
	virtual void load_Internal(const boost::json::object& obj) override;

	virtual void onBeginTargeted(FSISCharacter* hunter) override;

	virtual void onEndTargeted(FSISCharacter* hunter) override;

private:
	class SpriteComponent* m_marker = nullptr;
};