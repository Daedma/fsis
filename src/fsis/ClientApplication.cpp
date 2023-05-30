#include "ClientApplication.hpp"
#include "Engine.hpp"
#include "FSISHUD.hpp"
#include "core/InputEventListener.hpp"

ClientApplication::ClientApplication()
{
	AssetManager::init("resources.ini");
	AssetManager::initAnimGroups("animgroups.ini");

	m_client.init("client.ini");

	Canvas::init("FSIS");
	m_hud = Canvas::setHUD<FSISHUD>();
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