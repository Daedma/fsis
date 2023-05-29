#pragma once 

#include "gui/HUD.hpp"
#include <TGUI/TGUI.hpp>

class FSISHUD : public HUD
{
public:
	FSISHUD(sf::RenderWindow* window) : HUD(window) {}

	enum class States
	{
		AUTH,
		MAIN,
		CONTROL,
		FIGHT,
		ESC,
		WAVE,
		TOTAL,
		STATESCOUNT
	};

	void loadForms(const std::string& filename);

	void setInitState()
	{}

	void setWorld(class World* world)
	{
		m_world = world;
	}

	void setClient(class NetworkClient* client)
	{
		m_client = client;
	}

private:
	States m_curState = States::AUTH;

	States m_initState = States::AUTH;

	class World* m_world = nullptr;

	class NetworkClient* m_client = nullptr;

	tgui::Group::Ptr m_auth;

	tgui::Group::Ptr m_main;

	tgui::Group::Ptr m_control;

	tgui::Group::Ptr m_fight;

	tgui::Group::Ptr m_esc;

	tgui::Group::Ptr m_wave;

	tgui::Group::Ptr m_total;

private:
	void loadAuthorizationForm(const std::string& filename);

	void loadMainForm(const std::string& filename);

	void loadControlForm(const std::string& filename);

	void loadFightForm(const std::string& filename);

	void loadEscForm(const std::string& filename);

	void loadWaveForm(const std::string& filename);

	void loadTotalForm(const std::string& filename);

	void performAuth(const std::string& login, const std::string& pass);

};