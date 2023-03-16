#include "core/World.hpp"
#include "core/GameObject.hpp"

void World::registry(GameObject* obj)
{
	switch (obj->getTickGroup())
	{
	case TickGroups::PREPHYSIC:
		m_prephys.emplace(obj);
		break;
	case TickGroups::PHYSIC:
		m_phys.emplace(obj);
		break;
	case TickGroups::POSTPHYSIC:
		m_postphys.emplace(obj);
		break;
	default:
		return;
	}
}

void World::unregistry(GameObject* obj)
{
	switch (obj->getTickGroup())
	{
	case TickGroups::PREPHYSIC:
		m_prephys.erase(obj);
		break;
	case TickGroups::PHYSIC:
		m_phys.erase(obj);
		break;
	case TickGroups::POSTPHYSIC:
		m_postphys.erase(obj);
		break;
	default:
		return;
	}
}