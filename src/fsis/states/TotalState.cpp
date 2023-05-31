#include "states/TotalState.hpp"
#include "states/MenuState.hpp"
#include "NetworkClient.hpp"
#include "render/Canvas.hpp"
#include "gui/HUD.hpp"
#include "core/AssetManager.hpp"
#include <TGUI/AllWidgets.hpp>
#include <thread>
#include <iostream>

void TotalState::init(const MatchStats& stats)
{
	AssetManager::stopMusic();

	AssetManager::playMusic("menu_3.ogg");

	m_nextState = this;

	m_stats = stats;

	m_dataReceived = false;

	m_dataUpdated = false;

	m_error = false;

	auto form = Canvas::getHUD()->pushMenuItem("results");

	form
		->get<tgui::Label>("Label_Number_Wave")
		->setText(tgui::String("YOU SURVIVED ")
			+ tgui::String::fromNumber(m_stats.waves)
			+ tgui::String(" WAVES"));

	form
		->get<tgui::Label>("Label_Number_Score")
		->setText(tgui::String::fromNumber(m_stats.score));

	form
		->get<tgui::Label>("Label_Number_Kills")
		->setText(tgui::String::fromNumber(m_stats.kills));

	form
		->get<tgui::Button>("Button_Close")
		->onClick.disconnectAll();

	form
		->get<tgui::Button>("Button_Close")
		->onClick(
			[this]() {
				m_dataReceived = false;
				Canvas::getHUD()->popMenuItem();
				m_nextState = MenuState::getInstance();
				MenuState::getInstance()->init();
			}
	);

	sendStats();
}

void TotalState::tick(float)
{
	if (m_dataUpdated)
	{
		return;
	}
	else if (m_dataReceived && m_error)
	{
		Canvas::getHUD()
			->getCurrentMenuItem()
			->get<tgui::TextArea>("TextArea_List_Achievents")
			->setText("Failed to get data from server");
		m_dataUpdated = true;
	}
	else if (m_dataReceived)
	{
		tgui::TextArea::Ptr achievemets_TA = Canvas::getHUD()
			->getCurrentMenuItem()
			->get<tgui::TextArea>("TextArea_List_Achievents");
		for (const auto& i : m_achievements)
		{
			achievemets_TA->addText(i.description);
			achievemets_TA->addText(" +" + tgui::String::fromNumber(i.reward) + "\n");
		}
		m_dataUpdated = true;
	}
}

void TotalState::sendStats()
{
	m_dataReceived = false;
	m_error = false;
	std::thread{
		[this]() {
			m_error = !NetworkClient::sendMatchStatsToServer(m_stats);
			m_achievements = NetworkClient::getAchievementsFromServer();
			m_dataReceived = true;
		}
	}.detach();
}