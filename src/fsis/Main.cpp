#include "Sorcerer.hpp"
#include "Engine.hpp"
#include "FSISAction.hpp"


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
		AssetManager::initAnimGroups("groups.json");
		Canvas::init("FSIS");
		Canvas::setHUD<HUD>();
		m_world.setGameMode<GameMode>();

		Map* map = m_world.spawnMap<Map>();
		AssetManager::loadMap(map, "map (3).json");

		Sorcerer* hero = m_world.spawnActor<Sorcerer>(map->getSpawnPoint(0));
		hero->load("player.json");

		FollowCamera* cam = Canvas::createCamera<FollowCamera>();
		cam->setTarget(hero);
		cam->setScale(3000);

		Sorcerer* enemy = m_world.spawnActor<Sorcerer>(map->getSpawnPoint(40));
		enemy->load("enemy.json");
		AIController* ai = m_world.spawnController<AIController>();
		ai->setTarget(hero);
		ai->setAttackRange(enemy->getAttackRange());
		ai->bindAttackAction(FSISActions::ATTACK);
		ai->possess(enemy);

		PlayerController* controller = m_world.spawnController<PlayerController>();
		controller->bindKeyAction(PlayerController::KeyCode::E, FSISActions::ATTACK);
		controller->bindKeyAction(PlayerController::KeyCode::LShift, FSISActions::TOGGLE_RUN);
		controller->bindKeyAxis(PlayerController::KeyCode::W, +1.f, FSISActions::MOVE_FORWARD);
		controller->bindKeyAxis(PlayerController::KeyCode::S, -1.f, FSISActions::MOVE_FORWARD);
		controller->bindKeyAxis(PlayerController::KeyCode::D, +1.f, FSISActions::MOVE_RIGHT);
		controller->bindKeyAxis(PlayerController::KeyCode::A, -1.f, FSISActions::MOVE_RIGHT);
		controller->possess(hero);
	}

	void run()
	{
		std::cout << "Application started\n";
		sf::Clock tick;
		sf::RenderWindow& window = *Canvas::getWindow();
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
