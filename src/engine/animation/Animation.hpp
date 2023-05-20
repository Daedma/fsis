#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <EASTL/unique_ptr.h>
#include <EASTL/string.h>
#include <EASTL/functional.h>
#include <EASTL/vector.h>
#include <EASTL/iterator.h>
#include <EASTL/algorithm.h>
#include "core/GameObject.hpp"
#include "core/TransformTypes.hpp"

class Animation : public sf::Drawable, public GameObject
{
public:
	using NotifyEventHandlerSignature = eastl::function<void(float)>;

	struct Notify
	{
		uint8_t frame;
		eastl::string name;
		NotifyEventHandlerSignature handler = nullptr;

		static constexpr auto equal = [](const Notify& notify) {
			return [&notify](const Notify& other) {
				return notify.name == other.name;
			};
		};

		static constexpr auto isMyFrame = [](uint8_t frame) {
			return [frame](const Notify& notify) {return frame == notify.frame; };
		};
	};

	Animation(World* world) : GameObject(world), m_sprite(new sf::Sprite()) {}

	Animation() : m_sprite(new sf::Sprite()) {}

	void setTexture(sf::Texture* texture)
	{
		m_texture = texture;
		m_sprite->setTexture(*texture);
	}

	enum class Status
	{
		PLAYING, PAUSED, STOPPED
	};

	void play(bool bContinue = true);

	void stop();

	void pause();

	void setReverse(bool bReverse) { b_reverse = bReverse; }

	Status getStatus() const { return m_status; }

	bool isActive() const { return m_status == Status::PLAYING; }

	void setHeight(float height)
	{
		EASTL_ASSERT(height != 0 && m_texture);
		float ratio = SQRT_3 * height / m_initFrame.height;
		m_sprite->setScale(ratio, ratio);
	}

	void setInitFrame(sf::IntRect frame)
	{
		m_initFrame = frame;
	}

	void setFramesCount(uint8_t framesCount)
	{
		m_framesCount = framesCount;
	}

	void setFrequency(float freq)
	{
		m_period = 1.f / freq;
	}

	float getDuration()
	{
		return m_period * m_framesCount;
	}

	void setLoop(bool loop) { b_loop = loop; }

	uint8_t	getCurrentFrame() const { return m_curFrame; }

	void addNotify(const Notify& notify);

	void setNotifyHandler(const eastl::string& notify, NotifyEventHandlerSignature handler);

	// remove notify on request

	virtual void tick(float deltaSeconds) override;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	eastl::unique_ptr<sf::Sprite> m_sprite;

	float m_period;

	float m_elapsedTimeSinceFrameChange = 0.f;

	uint8_t m_framesCount = 0;

	uint8_t m_curFrame = 0;

	sf::IntRect m_initFrame;

	sf::Texture* m_texture = nullptr;

	Status m_status = Status::STOPPED;

	bool b_loop = true;

	eastl::vector<Notify> m_notifies;

	bool b_reverse = false;

private:
	void nextFrame()
	{
		sf::IntRect frame = m_initFrame;
		if (!b_reverse)
		{
			frame.left = m_initFrame.width * m_curFrame + m_initFrame.left;
		}
		else
		{
			frame.left = m_initFrame.width * (m_framesCount - m_curFrame) + m_initFrame.left;
		}
		m_sprite->setTextureRect(frame);
	}

	void handleNotifies()
	{
		eastl::vector<Notify> curFrameNotifies;
		eastl::copy_if(m_notifies.begin(), m_notifies.end(),
			eastl::back_inserter(curFrameNotifies), Notify::isMyFrame(m_curFrame));
		for (auto& i : curFrameNotifies)
		{
			i.handler(m_period * m_curFrame);
		}
	}
};