#include "components/CharacterAnimComponent.hpp"
#include "components/MovementComponent.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <EASTL/algorithm.h>
#include <cmath>

Animation* CharacterAnimComponent::addOrientedAnimation(uint8_t sector, uint32_t group)
{
	EASTL_ASSERT_MSG(group != UNDIRECTED_GROUP, "0 group is reserved for undirected animations");
	Animation* newAnimation = m_animations.emplace_back(new Animation()).get();
	newAnimation->pause();
	if (m_groups.count(group) == 0)
	{
		eastl::fill(m_groups[group].begin(), m_groups[group].end(), nullptr);
	}
	m_groups[group][sector] = newAnimation;
	return newAnimation;
}

Animation* CharacterAnimComponent::addUndirectedAnimation(uint32_t id)
{
	Animation* newAnimation = m_animations.emplace_back(new Animation()).get();
	newAnimation->pause();
	m_undirectedAnimations[id] = newAnimation;
	return newAnimation;
}

Animation* CharacterAnimComponent::playAnimation(uint32_t id)
{
	EASTL_ASSERT_MSG(m_undirectedAnimations.count(id), "Unexpected animation id");
	stopAnimation();
	Animation* anim = m_undirectedAnimations[id];
	anim->play();
	m_activeAnimation = anim;
	m_activeGroup = UNDIRECTED_GROUP;
	return anim;
}

Animation* CharacterAnimComponent::playAnimation(uint8_t sector, uint32_t group)
{
	EASTL_ASSERT_MSG(m_groups.count(group), "Unexpected group");
	EASTL_ASSERT_MSG(group != UNDIRECTED_GROUP, "You cannot play undirected animation with this method");
	Animation* anim = m_groups[group][sector];
	EASTL_ASSERT_MSG(anim, "Null animation");
	stopAnimation();
	anim->play();
	m_activeAnimation = anim;
	m_activeGroup = group;
	return anim;
}

Animation* CharacterAnimComponent::stopAnimation()
{
	if (m_activeAnimation)
	{
		m_activeAnimation->stop();
	}
	return m_activeAnimation;
}

void CharacterAnimComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	EASTL_ASSERT(m_activeAnimation);
	m_activeAnimation->draw(target, states);
}

void CharacterAnimComponent::tick(float deltaSeconds)
{
	Vector3f speed = m_character->getMovementComponent()->getCurrentSpeed();
	bool isNoMovement = mathter::IsNullvector(Vector2f(speed.xy));
	updateOrientation();
	// Если активная группа не является группой движения
	if (m_activeGroup != MOVEMENT_GROUP)
	{
		// Если нужно остановить анимацию движения
		if (b_movementStopAnimation)
		{
			// Если текущая скорость равна нулю и активная анимация уже запущена
			if (isNoMovement && m_activeAnimation->isActive())
			{
				return;
			}
			// Если текущая скорость равна нулю и активная анимация не запущена
			if (isNoMovement && !m_activeAnimation->isActive())
			{
				playAnimationGroup(MOVEMENT_GROUP);
			}
			// Если текущая скорость не равна нулю
			if (!isNoMovement)
			{
				playAnimationGroup(MOVEMENT_GROUP);
			}
		}
		// Если не нужно остановить анимацию движения
		else
		{
			// Если активная анимация уже запущена
			if (m_activeAnimation->isActive())
			{
				return;
			}
			// Если активная анимация не запущена
			else
			{
				playAnimationGroup(MOVEMENT_GROUP);
			}
		}
	}
	// Если активная группа является группой движения
	else
	{
		// Если текущая скорость равна нулю, то остановить текущую анимацию
		if (isNoMovement)
		{
			stopAnimation();
		}
		// Если текущая скорость не равна нулю
		else
		{
			if (!m_activeAnimation->isActive())
			{
				m_activeAnimation->play();
			}
			// Установить флаг обратного направления анимации в зависимости от направления движения
			bool isDifferentDirections = mathter::Dot(speed,
				getOwner()->getForwardVector()) < 0;
			m_activeAnimation->setReverse(isDifferentDirections);
		}
	}
}

uint8_t CharacterAnimComponent::getSector() const
{
	static constexpr float PI_DIV_8 = 0.39269908169872415480783042290994f;
	static constexpr float _4_DIV_PI = 1.2732395447351626861510701069801f;
	Vector3f orientation = getOwner()->getForwardVector();
	float angle = std::atan2(orientation.y, orientation.x);
	uint8_t nonNormalizedSector = std::floor((angle + PI_DIV_8) * _4_DIV_PI) + 8;
	return nonNormalizedSector % 8;
}

void CharacterAnimComponent::updateOrientation()
{
	if (m_activeGroup != UNDIRECTED_GROUP)
	{
		Animation* fitAnimation = m_groups[m_activeGroup][getSector()];
		if (fitAnimation != m_activeAnimation)
		{
			m_activeAnimation->stop();
			m_activeAnimation = fitAnimation;
			fitAnimation->play();
		}
	}
}

void CharacterAnimComponent::onAttached()
{
	PrimitiveComponent::onAttached();
	m_character = dynamic_cast<Character*>(getOwner());
}

bool CharacterAnimComponent::checkInitializationCompleteness() const
{
	bool status = true;
	status = status && m_groups.find(MOVEMENT_GROUP) != m_groups.end();
	status = status && m_character;
	for (const auto& i : m_groups)
	{
		for (const auto& j : i.second)
		{
			status = status && j;
		}
	}
	return status;
}

void CharacterAnimComponent::setInitState(uint32_t group)
{
	for (auto& i : m_animations)
	{
		i->initWorld(getWorld());
	}
	playAnimationGroup(group);
}