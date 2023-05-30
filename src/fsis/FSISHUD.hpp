#pragma once 

#include "gui/HUD.hpp"
#include <TGUI/TGUI.hpp>
#include <EASTL/stack.h>

class FSISHUD : public HUD
{
public:
	FSISHUD(sf::RenderWindow* window) : HUD(window) {}
};