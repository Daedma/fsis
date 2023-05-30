#pragma once

#include "State.hpp"
#include "core/World.hpp"
#include "Entity.hpp"
#include <EASTL/optional.h>
#include <TGUI/AllWidgets.hpp>

class GameState : public State
{
	GameState() = default;

public:
	static GameState* getInstance()
	{
		static GameState instance;
		return &instance;
	}

	virtual void tick(float deltaSeconds) override;

	void init();

private:
	eastl::optional<World> m_world;

	tgui::ProgressBar::Ptr m_specialAttack_PB;

	tgui::ProgressBar::Ptr m_specialMode_PB;

	tgui::ProgressBar::Ptr m_specialAbility_PB;

	tgui::ProgressBar::Ptr m_health_PB;

	tgui::Picture::Ptr m_entity_P;

	tgui::TextArea::Ptr m_wave_TA;

	tgui::TextArea::Ptr m_score_TA;

	class Sorcerer* m_player = nullptr;

	class FSISGameMode* m_mode = nullptr;

	Entity m_lastEntity = Entity::INFERNAL;

private:
	void updateHUD();
};