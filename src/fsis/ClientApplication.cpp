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
	m_hud->setWorld(getWorld());
	m_hud->setInitState();
}

void ClientApplication::run()
{
	sf::Clock ticks;
	while (Canvas::getWindow()->isOpen())
	{
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
		float deltaSeconds = ticks.restart().asSeconds();
		m_world.tick(deltaSeconds);
		m_hud->tick(deltaSeconds);
		Canvas::draw();
	}
}