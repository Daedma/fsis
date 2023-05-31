#include "states/AuthState.hpp"
#include "states/MenuState.hpp"
#include "core/AssetManager.hpp"
#include "NetworkClient.hpp"
#include "render/Canvas.hpp"
#include "gui/HUD.hpp"
#include <TGUI/TGUI.hpp>
#include <thread>

void AuthState::init()
{
	AssetManager::playMusic("menu_3.ogg");
	m_nextState = this;

	auto form = Canvas::getHUD()->pushMenuItem("authorization");

	form->get<tgui::Button>("Button_LOGIN")
		->onClick.disconnectAll();

	form->get<tgui::Button>("Button_LOGIN")
		->onClick(&AuthState::tryToLogin, this);

	form->get<tgui::Button>("Button_Close_Window")
		->onClick.disconnectAll();

	form->get<tgui::Button>("Button_Close_Window")
		->onClick(&sf::RenderWindow::close, Canvas::getWindow());

	form->get<tgui::Label>("Label_Wrong_Input")
		->setVisible(false);

}

void AuthState::tryToLogin()
{
	auto form = Canvas::getHUD()->getCurrentMenuItem();
	std::string username = form->get<tgui::EditBox>("EditBox_Username")->getText().toStdString();
	std::string password = form->get<tgui::EditBox>("EditBox_Password")->getText().toStdString();
	m_responseReceived = false;
	std::thread{
		[this, username, password]() {
			NetworkClient::Status status = NetworkClient::auth(username, password);
			m_responseReceived = true;
			m_successfulLogin = status == NetworkClient::Status::SUCCESS;
		}
	}.detach();
}

void AuthState::tick(float deltaSeconds)
{
	if (m_responseReceived)
	{
		if (m_successfulLogin)
		{
			MenuState* next = MenuState::getInstance();
			next->init();
			m_nextState = next;
		}
		else
		{
			Canvas::getHUD()
				->getCurrentMenuItem()
				->get<tgui::Label>("Label_Wrong_Input")
				->setVisible(!m_successfulLogin);
		}

	}
}