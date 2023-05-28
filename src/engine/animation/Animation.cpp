#include "animation/Animation.hpp"
#include <EASTL/algorithm.h>
#include <SFML/Graphics/RenderTarget.hpp>

void Animation::play(bool bContinue)
{
	m_status = Status::PLAYING;
	nextFrame();
	if (!bContinue)
	{
		m_curFrame = 0;
		m_elapsedTimeSinceFrameChange = 0.f;
	}
}

void Animation::stop()
{
	m_status = Status::STOPPED;
	m_curFrame = 0;
	m_elapsedTimeSinceFrameChange = 0.f;
	nextFrame();
}

void Animation::pause()
{
	m_status = Status::PAUSED;
	nextFrame();
}

void Animation::addNotify(const Notify& notify)
{
	if (eastl::none_of(m_notifies.begin(), m_notifies.end(), [&notify](const Notify& other) {
		return other.name == notify.name;
		}))
	{
		m_notifies.emplace_back(notify);
	}
}

void Animation::setNotifyHandler(const eastl::string& notify, NotifyEventHandlerSignature handler)
{
	auto pos = eastl::find_if(m_notifies.begin(), m_notifies.end(), [&notify](const Notify& val) {
		return notify == val.name;
		});
	EASTL_ASSERT(pos != m_notifies.end());
	pos->handler = handler;
}

void Animation::tick(float deltaSeconds)
{
	switch (m_status)
	{
	case Status::PAUSED: case Status::STOPPED:
		return;
	case Status::PLAYING: default: break;
	}
	m_elapsedTimeSinceFrameChange += deltaSeconds;
	if (m_elapsedTimeSinceFrameChange > m_period)
	{
		m_curFrame = (++m_curFrame) % m_framesCount;
		if (m_curFrame == 0 && !b_loop)
		{
			m_curFrame = m_framesCount - 1;
			pause();
		}
		else
		{
			nextFrame();
			handleNotifies();
			m_elapsedTimeSinceFrameChange = 0.f;
		}
	}
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_sprite, states);
}