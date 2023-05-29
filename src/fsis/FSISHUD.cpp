#include "FSISHUD.hpp"
#include "NetworkClient.hpp"
#include <boost/json.hpp>
#include <fstream>
#include "Engine.hpp"
#include <thread>

void FSISHUD::loadForms(const std::string& filename)
{
	namespace json = boost::json;

	std::ifstream ifs(filename);
	json::object formsInfo = json::parse(ifs).as_object();

	const json::string& initForm = formsInfo.at("init").as_string();
	if (initForm == "auth")
	{
		m_initState = States::AUTH;
	}
	else if (initForm == "main")
	{
		m_initState = States::MAIN;
	}
	else if (initForm == "fight")
	{
		m_initState = States::FIGHT;
	}

	const json::object& forms = formsInfo.at("forms").as_object();
	loadAuthorizationForm(forms.at("auth").as_string().c_str());
	loadMainForm(forms.at("main").as_string().c_str());
	loadControlForm(forms.at("control").as_string().c_str());
	loadFightForm(forms.at("fight").as_string().c_str());
	loadEscForm(forms.at("esc").as_string().c_str());
	loadWaveForm(forms.at("wave").as_string().c_str());
	loadTotalForm(forms.at("total").as_string().c_str());
}

void FSISHUD::loadAuthorizationForm(const std::string& filename)
{
	m_auth = tgui::Group::create();
	m_auth->loadWidgetsFromFile(filename);
	tgui::Button::Ptr login = m_auth->get<tgui::Button>("Button_LOGIN");
	tgui::Label::Ptr wrongInput = m_auth->get<tgui::Label>("Label_Wrong_Input");
	tgui::EditBox::Ptr username = m_auth->get<tgui::EditBox>("EditBox_Username");
	tgui::EditBox::Ptr password = m_auth->get<tgui::EditBox>("EditBox_Password");
	tgui::Button::Ptr closeWindow = m_auth->get<tgui::Button>("Button_Close_Window");

	wrongInput->setVisible(false);

	closeWindow->onClick(&sf::RenderWindow::close, Canvas::getWindow());

	login->onClick([this, username, password, wrongInput, login]() {
		NetworkClient::Status status = m_client->auth(username->getText().toStdString(), password->getText().toStdString());
		if (status == NetworkClient::Status::FAILURE)
		{
			password->setText("");
			wrongInput->setVisible(true);
		}
		else
		{
			m_auth->setVisible(false);
			m_main->setVisible(true);
			m_curState = States::MAIN;
		}
		});

	add(m_auth);
	m_auth->setVisible(false);
}

void FSISHUD::loadMainForm(const std::string& filename)
{
	m_main = tgui::Group::create();
	m_main->loadWidgetsFromFile(filename);
	tgui::Button::Ptr fight = m_main->get<tgui::Button>("Button_Fight");
	tgui::Button::Ptr control = m_main->get<tgui::Button>("Button_Control");
	tgui::Button::Ptr exit = m_main->get<tgui::Button>("Button_Exit");


	fight->onClick([this]() {
		m_main->setVisible(false);
		m_fight->setVisible(true);

		});
}