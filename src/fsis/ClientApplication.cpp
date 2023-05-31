#include "ClientApplication.hpp"
#include "core/InputEventListener.hpp"
#include "core/AssetManager.hpp"
#include "render/Canvas.hpp"
#include "states/AuthState.hpp"
#include "gui/HUD.hpp"
#include "NetworkClient.hpp"
#include <iostream>

ClientApplication::ClientApplication()
{
	AssetManager::init("resources.ini");
	std::cout << "Recources initialized\n";
	AssetManager::initAnimGroups("animgroups.ini");
	std::cout << "Animation groups initialized\n";

	NetworkClient::init("services.ini");
	std::cout << "Network client initialized\n";

	Canvas::init("FSIS");
	Canvas::setHUD<HUD>();
	Canvas::getHUD()->loadForms("forms.ini");
	std::cout << "Forms are loaded\n";

	m_curState = AuthState::getInstance();
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