#include "states/GameState.hpp"
#include "states/TotalState.hpp"
#include "render/Canvas.hpp"
#include "gui/HUD.hpp"
#include "core/AssetManager.hpp"
#include "Sorcerer.hpp"
#include "FSISGameMode.hpp"
#include <string>

void GameState::init()
{
	m_nextState = this;

	auto form = Canvas::getHUD()->pushMenuItem("fight");

	m_specialAttack_PB = form->get<tgui::ProgressBar>("ProgressBar_OA");

	m_specialMode_PB = form->get<tgui::ProgressBar>("ProgressBar_CP");

	m_specialAbility_PB = form->get<tgui::ProgressBar>("ProgressBar_CC");

	m_health_PB = form->get<tgui::ProgressBar>("ProgressBar_Health");

	m_entity_P = form->get<tgui::Picture>("Picture_Ball");

	m_wave_TA = form->get<tgui::TextArea>("TextArea_Number_Wave");

	m_score_TA = form->get<tgui::TextArea>("TextArea_Number_Score");

	Canvas::getWindow()->setMouseCursorVisible(false);

	m_world.emplace();
	m_mode = m_world->setGameMode<FSISGameMode>();
	m_player = m_mode->getPlayer();
	m_world->start();
}

void GameState::tick(float deltaSeconds)
{
	m_world->tick(deltaSeconds);
	if (m_world->isFinished())
	{
		MatchStats stats = m_mode->getMatchStats();
		m_player = nullptr;
		m_mode = nullptr;
		m_world.reset();
		Canvas::getHUD()->popMenuItem();
		m_nextState = TotalState::getInstance();
		TotalState::getInstance()->init(stats);
	}
	else
	{
		updateHUD();
	}
}

void GameState::updateHUD()
{
	m_specialAttack_PB->setValue(m_player->getSpecialAttackCooldown() * 100.f);
	m_specialMode_PB->setValue(m_player->getSpecialModeCooldown() * 100.f);
	m_specialAbility_PB->setValue(m_player->getSpecialAbilityCooldown() * 100.f);
	m_health_PB->setValue(m_player->getHP() / m_player->getMaxHP() * 100.f);

	if (m_player->getEntity() != m_lastEntity)
	{
		switch (m_player->getEntity())
		{
		case Entity::INFERNAL:
			m_entity_P
				->getRenderer()
				->setTexture(*AssetManager::loadTextureTGUI("Ball_Fair.png"));
			break;
		case Entity::DISEASED:
			m_entity_P
				->getRenderer()
				->setTexture(*AssetManager::loadTextureTGUI("Ball_Warped_sterm.png"));
			break;
		case Entity::DIVINE:
			m_entity_P
				->getRenderer()
				->setTexture(*AssetManager::loadTextureTGUI("Ball_Glowstone.png"));
			break;
		case Entity::UNHOLY:
			m_entity_P
				->getRenderer()
				->setTexture(*AssetManager::loadTextureTGUI("Ball_Crying_obsidian.png"));
			break;
		case Entity::PURIFIED:
			m_entity_P
				->getRenderer()
				->setTexture(*AssetManager::loadTextureTGUI("Ball_Sculk.png"));
			break;
		case Entity::QUINTESSENCE:
			m_entity_P
				->getRenderer()
				->setTexture(*AssetManager::loadTextureTGUI("Ball_black_OA.png"));
			break;
		case Entity::NONE:
		default:
			break;
		}
		m_lastEntity = m_player->getEntity();
	}

	m_wave_TA->setText(std::to_string(m_mode->getCurrentWave()));
	m_score_TA->setText(std::to_string(m_mode->getScore()));
}