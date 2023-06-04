/**
 * @file Animation.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains declaration of Animation class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
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

/**
 * @brief Animation is rolling frame set.
 * For single animation one texture is used. Frames are located in single line.
 * Animations provide notify system for bind handler to specified frame change.
 * Animation is simple primitive to construct more complex animations.
 * @sa CharacterAnimComponent
 */
class Animation : public sf::Drawable, public GameObject
{
public:
	using NotifyEventHandlerSignature = eastl::function<void(float duration)>;

	struct Notify
	{
		uint8_t frame;
		eastl::string name;
		NotifyEventHandlerSignature handler = nullptr;
	};

	/**
	 * @brief Construct a new Animation object
	 * @param world
	 */
	Animation(World* world) : GameObject(world), m_sprite(new sf::Sprite())
	{}

	/**
	 * @brief Construct a new Animation object
	 */
	Animation() : m_sprite(new sf::Sprite()) {}

	/**
	 * @brief Set the Texture
	 * @param texture
	 */
	void setTexture(sf::Texture* texture)
	{
		m_texture = texture;
		m_sprite->setTexture(*texture);
	}

	enum class Status
	{
		PLAYING, PAUSED, STOPPED
	};

	/**
	 * @brief Play animation
	 * @param bContinue Continue from current frame?
	 */
	void play(bool bContinue = true);

	/**
	 * @brief Stop animation.
	 * Set current frame to 0.
	 */
	void stop();

	/**
	 * @brief Pause animation.
	 * Doesn't change current frame.
	 */
	void pause();

	/**
	 * @brief Set the Reverse.
	 * @param bReverse Play animation vise versa or not?
	 */
	void setReverse(bool bReverse) { b_reverse = bReverse; }

	/**
	 * @brief Get the Status of this object
	 * @return Status
	 */
	Status getStatus() const { return m_status; }

	/**
	 * @brief Is animation playing?
	 * @return true animation is playing
	 * @return false animation is paused or stopped
	 */
	bool isActive() const { return m_status == Status::PLAYING; }

	/**
	 * @brief Set the Height
	 * @param height in world
	 */
	void setHeight(float height)
	{
		EASTL_ASSERT(height != 0 && m_texture);
		float ratio = SQRT_3 * height / m_initFrame.height;
		m_sprite->setScale(ratio, ratio);
	}

	/**
	 * @brief Set the Init Frame
	 * @param frame
	 */
	void setInitFrame(sf::IntRect frame)
	{
		m_initFrame = frame;
	}

	/**
	 * @brief Set the Frames Count
	 * @param framesCount
	 */
	void setFramesCount(uint8_t framesCount)
	{
		m_framesCount = framesCount;
	}

	/**
	 * @brief Set the Frequency
	 * @param freq
	 */
	void setFrequency(float freq)
	{
		m_period = 1.f / freq;
	}

	/**
	 * @brief Get the Duration of this object
	 * @return float
	 */
	float getDuration()
	{
		return m_period * m_framesCount;
	}

	/**
	 * @brief Set the Loop
	 * @param loop
	 */
	void setLoop(bool loop) { b_loop = loop; }

	/**
	 * @brief Set the Frame
	 * @param frame
	 */
	void setFrame(uint8_t frame)
	{
		m_curFrame = eastl::max<uint8_t>(frame, m_framesCount - 1);
		nextFrame();
	}

	/**
	 * @brief Get the Current Frame of this object
	 * @return uint8_t
	 */
	uint8_t	getCurrentFrame() const { return m_curFrame; }

	/**
	 * @brief Add notify object to animation
	 * @param notify
	 */
	void addNotify(const Notify& notify);

	/**
	 * @brief Set the handler of notify by name
	 * @param notify
	 * @param handler
	 */
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
			frame.left = m_initFrame.width * (m_framesCount - m_curFrame - 1) + m_initFrame.left;
		}
		m_sprite->setTextureRect(frame);
		updateOrigin(frame);
	}

	void handleNotifies()
	{
		eastl::vector<Notify> curFrameNotifies;
		eastl::copy_if(m_notifies.begin(), m_notifies.end(),
			eastl::back_inserter(curFrameNotifies), [this](const Notify& notify) {
				return notify.frame == m_curFrame;
			});
		for (auto& i : curFrameNotifies)
		{
			i.handler(m_period * m_curFrame);
		}
	}

	void updateOrigin(const sf::IntRect& frame)
	{
		m_sprite->setOrigin(frame.width / 2, frame.height * (1 - INVSQRT_3));
	}
};