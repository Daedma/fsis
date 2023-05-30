#include "gui/HUD.hpp"
#include <SFML/Graphics/RenderStates.hpp>
#include <boost/json.hpp>
#include <fstream>

void HUD::processInput(const sf::Event& event)
{
	tgui::GuiSFML::handleEvent(event);
}

void HUD::loadForms(const char* filename)
{
	namespace json = boost::json;

	std::ifstream ifs(filename);
	json::array forms = json::parse(filename).as_object().at("forms").as_array();

	for (const auto& i : forms)
	{
		tgui::Group::Ptr last = m_menuSet[i.as_object().at("name").as_string().c_str()];
		last->loadWidgetsFromFile(i.as_object().at("file").as_string().c_str());
		add(last);
		last->setVisible(false);
	}
}

