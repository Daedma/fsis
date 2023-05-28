#include "Sorcerer.hpp"
#include "Monster.hpp"
#include "Engine.hpp"
#include "FSISAction.hpp"
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

		m_world.setGameMode<GameMode>();
		LOG("Game mode customized");

		Map* map = m_world.spawnMap<Map>();
		AssetManager::loadMap(map, "map.json");
		LOG("Map loaded");

		Sorcerer* hero = m_world.spawnActor<Sorcerer>(map->getSpawnPoint(0));
		hero->load("resources/creatures/player.json");
		LOG("Player spawned");

		FollowCamera* cam = Canvas::createCamera<FollowCamera>();
		cam->setTarget(hero);
		cam->setScale(3000);
		LOG("Camera customized");

		Monster* enemy = m_world.spawnActor<Monster>(map->getSpawnPoint(40));
		enemy->load("resources/creatures/enemy.json");
		enemy->setTarget(hero);
		LOG("Enemy spawned");

		AIController* ai = m_world.spawnController<AIController>();
		ai->setTarget(hero);
		ai->setAttackRange(enemy->getAttackRange());
		ai->bindAttackAction(FSISActions::ATTACK);
		ai->possess(enemy);
		LOG("Enemy are possessed by AI controller");

		PlayerController* controller = m_world.spawnController<PlayerController>();
		controller->bindKeyAction(PlayerController::KeyCode::Q, FSISActions::NEXT_TARGET);
		controller->bindKeyAction(PlayerController::KeyCode::Num1, FSISActions::SP_ABILITY);
		controller->bindKeyAction(PlayerController::KeyCode::Num2, FSISActions::SP_ATTACK);
		controller->bindKeyAction(PlayerController::KeyCode::Num3, FSISActions::SP_MODE);
		controller->bindKeyAction(PlayerController::KeyCode::E, FSISActions::ATTACK);
		controller->bindKeyAction(PlayerController::KeyCode::LShift, FSISActions::TOGGLE_RUN);
		controller->bindKeyAxis(PlayerController::KeyCode::W, +1.f, FSISActions::MOVE_FORWARD);
		controller->bindKeyAxis(PlayerController::KeyCode::S, -1.f, FSISActions::MOVE_FORWARD);
		controller->bindKeyAxis(PlayerController::KeyCode::D, +1.f, FSISActions::MOVE_RIGHT);
		controller->bindKeyAxis(PlayerController::KeyCode::A, -1.f, FSISActions::MOVE_RIGHT);
		controller->possess(hero);
		LOG("Player are possessed by controller");
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
