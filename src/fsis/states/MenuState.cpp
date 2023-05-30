#include "states/MenuState.hpp"
#include "render/Canvas.hpp"
#include "gui/HUD.hpp"
#include <TGUI/AllWidgets.hpp>
#include "states/GameState.hpp"

void MenuState::init()
{
	auto form = Canvas::getHUD()->pushMenuItem("menu");

	form->get<tgui::Button>("Button_Exit")
		->onClick(&sf::RenderWindow::close, Canvas::getWindow());

	form->get<tgui::Button>("Button_Fight")
		->onClick(&MenuState::toFight, this);

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
		->onClick([]() {
		Canvas::getHUD()->popMenuItem();
			});
}

void MenuState::tick(float) {}