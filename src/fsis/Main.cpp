#include "Sorcerer.hpp"
#include "Monster.hpp"
#include "Engine.hpp"
#include "FSISAction.hpp"
#include "FSISGameMode.hpp"
#include "log.hpp"


void* operator new[](size_t size, const char* pName, int flags, unsigned debugFlags, const char* file, int line) {
	return new uint8_t[size];
	};

void* operator new[](size_t size, size_t alignment, size_t alignmentOffset, const char* pName, int flags, unsigned debugFlags, const char* file, int line) {
	return new uint8_t[size];
	};

class ClientApplication
{
	World m_world;

public:
	ClientApplication(unsigned width, unsigned height)
	{
		AssetManager::init("resources.ini");
		AssetManager::initAnimGroups("animgroups.ini");
		LOG("Assets initialization was succesfully");

		Canvas::init("FSIS");
		Canvas::setHUD<HUD>();
		LOG("Canvas customized");

		m_world.setGameMode<FSISGameMode>();
		LOG("Game mode customized");
	}

	void run()
	{
		std::cout << "Application started\n";
		sf::RenderWindow& window = *Canvas::getWindow();
		m_world.start();
		sf::Clock tick;
		while (!m_world.isFinished())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				InputEventListener::handleEvent(event);
				switch (event.type)
				{
				case sf::Event::EventType::Closed:
					window.close();
					m_world.finish();
					break;
				default:
					break;
				}
			}
			m_world.tick(tick.restart().asSeconds());
			Canvas::draw();
		}
		std::cout << "Application finished\n";
	}
};

int main(int argc, char const* argv[])
{
	ClientApplication app(800, 600);
	app.run();
	return 0;
}
