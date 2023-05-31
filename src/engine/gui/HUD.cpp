#include "gui/HUD.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include <boost/json.hpp>
#include <fstream>
#include <iostream>

void HUD::processInput(const sf::Event& event)
{
	tgui::GuiSFML::handleEvent(event);
}

void HUD::loadForms(const char* filename)
{
	namespace json = boost::json;

	std::ifstream ifs(filename);
	json::array forms = json::parse(ifs).as_object().at("forms").as_array();

	for (const auto& i : forms)
	{
		eastl::string name = i.as_object().at("name").as_string().c_str();
		m_menuSet[name] = tgui::Group::create();
		m_menuSet[name]->loadWidgetsFromFile(i.as_object().at("file").as_string().c_str());
		m_menuSet[name]->setVisible(false);
		add(m_menuSet[name]);
	}
}
