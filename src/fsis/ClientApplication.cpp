#include "ClientApplication.hpp"
#include "core/InputEventListener.hpp"
#include "core/AssetManager.hpp"
#include "render/Canvas.hpp"
#include "states/AuthState.hpp"
#include "gui/HUD.hpp"
#include "NetworkClient.hpp"

ClientApplication::ClientApplication()
{
	AssetManager::init("resources.ini");
	AssetManager::initAnimGroups("animgroups.ini");

	NetworkClient::init("client.ini");

	Canvas::init("FSIS");
	Canvas::setHUD<HUD>();
	Canvas::getHUD()->loadForms("forms.ini");

	m_curState = AuthState::getInstance();
}

void ClientApplication::run()
{
	sf::Clock ticks;
	while (Canvas::getWindow()->isOpen())
	{
		m_curState = m_curState->nextState(); // Update state
		sf::Event event;
		switch (event.type)
		{
			while (Canvas::getWindow()->pollEvent(event))
			{
				InputEventListener::handleEvent(event);
				if (event.type == sf::Event::Closed)
				{
					Canvas::getWindow()->close();
				}
			}
		}
		m_curState->tick(ticks.restart().asSeconds());
		Canvas::draw();
	}
}