#include "states/MenuState.hpp"
#include "render/Canvas.hpp"
#include "core/AssetManager.hpp"
#include "gui/HUD.hpp"
#include <TGUI/AllWidgets.hpp>
#include "states/GameState.hpp"

void MenuState::init()
{
	AssetManager::playMusic("menu_3.ogg")->setLoop(true);

	m_nextState = this;

	auto form = Canvas::getHUD()->pushMenuItem("menu");

	form->get<tgui::Button>("Button_Exit")
		->onClick.disconnectAll();

	form->get<tgui::Button>("Button_Exit")
		->onClick(&sf::RenderWindow::close, Canvas::getWindow());

	form->get<tgui::Button>("Button_Fight")
		->onClick.disconnectAll();

	form->get<tgui::Button>("Button_Fight")
		->onClick(&MenuState::toFight, this);

	form->get<tgui::Button>("Button_Control")
		->onClick.disconnectAll();

	form->get<tgui::Button>("Button_Control")
		->onClick(&MenuState::showControl, this);
}

void MenuState::toFight()
{
	GameState* next = GameState::getInstance();
	next->init();
	m_nextState = next;
}

void MenuState::showControl()
{
	auto form = Canvas::getHUD()->pushMenuItem("control");

	form->get<tgui::Button>("Button_Close")
		->onClick.disconnectAll();

	form->get<tgui::Button>("Button_Close")
		->onClick([]() {
		Canvas::getHUD()->popMenuItem();
			});
}

void MenuState::tick(float) {}