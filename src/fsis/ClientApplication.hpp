#pragma once

#include "NetworkClient.hpp"
#include "core/World.hpp"

class ClientApplication
{
public:
	ClientApplication();

	void run();

	World* getWorld()
	{
		return &m_world;
	}

private:
	class FSISHUD* m_hud;

	NetworkClient m_client;

	World m_world;
};