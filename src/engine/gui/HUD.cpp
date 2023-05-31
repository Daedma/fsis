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
		tgui::Group::Ptr last = m_menuSet[i.as_object().at("name").as_string().c_str()];
		last = tgui::Group::create();
		last->loadWidgetsFromFile(i.as_object().at("file").as_string().c_str());
		add(last);
		last->setVisible(false);
	}
}

