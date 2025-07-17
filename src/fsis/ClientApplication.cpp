#include "ClientApplication.hpp"
#include "core/InputEventListener.hpp"
#include "core/AssetManager.hpp"
#include "render/Canvas.hpp"
#include "states/AuthState.hpp"
#include "states/MenuState.hpp"
#include "gui/HUD.hpp"
#include "NetworkClient.hpp"
#include <iostream>

ClientApplication::ClientApplication()
{
	AssetManager::init("resources.ini");
	AssetManager::initAnimGroups("animgroups.ini");

	NetworkClient::init("services.ini");

	Canvas::init("FSIS");
	Canvas::setHUD<HUD>();
	Canvas::getHUD()->loadForms("forms.ini");

#ifdef FSIS_NO_AUTH	
	m_curState = MenuState::getInstance();
	MenuState::getInstance()->init();
#else
	m_curState = AuthState::getInstance();
	AuthState::getInstance()->init();
#endif
}

void ClientApplication::run()
{
	sf::Clock ticks;
	while (Canvas::getWindow()->isOpen())
	{
		m_curState = m_curState->nextState(); // Update state
		sf::Event event;
		while (Canvas::getWindow()->pollEvent(event))
		{
			InputEventListener::handleEvent(event);
			if (event.type == sf::Event::Closed)
			{
				Canvas::getWindow()->close();
			}
		}
		m_curState->tick(ticks.restart().asSeconds());
		Canvas::draw();
	}
}