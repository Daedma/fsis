/**
 * @file HUD.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains the declaration of HUD class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <TGUI/Backends/SFML.hpp>
#include <TGUI/Widgets/Group.hpp>
#include <EASTL/stack.h>
#include <EASTL/string.h>
#include <EASTL/hash_map.h>
#include <core/InputEventListener.hpp>

/**
 * @brief A class represents GUI in program
 */
class HUD : public tgui::GuiSFML, public InputEventListener
{
public:
	HUD(sf::RenderWindow* window) : tgui::GuiSFML(*window) {}

	virtual void processInput(const sf::Event& event) override;

	tgui::Group::Ptr pushMenuItem(const eastl::string& menu)
	{
		if (!m_menuStack.empty())
		{
			m_menuStack.top()->setVisible(false);
		}
		m_menuStack.push(m_menuSet.at(menu));
		m_menuStack.top()->setVisible(true);
		return m_menuStack.top();
	}

	tgui::Group::Ptr popMenuItem()
	{
		if (m_menuStack.empty())
		{
			return nullptr;
		}
		tgui::Group::Ptr old = m_menuStack.top();
		old->setVisible(false);
		m_menuStack.pop();
		if (!m_menuStack.empty())
		{
			m_menuStack.top()->setVisible(true);
		}
		return old;
	}

	tgui::Group::Ptr getCurrentMenuItem()
	{
		return m_menuStack.top();
	}

	void loadForms(const char* filename);

private:
	eastl::stack<tgui::Group::Ptr> m_menuStack;

	eastl::hash_map<eastl::string, tgui::Group::Ptr> m_menuSet;
};