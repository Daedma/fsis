/**
 * @file AudioComponent.hpp
 * @author Damir Khismatov (hdamir163@gmail.com)
 * @brief Contains declaration of AudioComponent class
 * @version 1.0
 * @date 2023-06-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include "components/ActorComponent.hpp"
#include <EASTL/unique_ptr.h>
#include <EASTL/string.h>
#include <EASTL/functional.h>
#include <EASTL/hash_map.h>
#include <SFML/Audio/Sound.hpp>

/**
 * @brief Provides sounds support for actor.
 * Can manipulate with multiple sounds.
 */
class AudioComponent : public ActorComponent
{
public:
	using OnPlayingFinishedSignature = eastl::function<void(float duration)>;

	AudioComponent() = default;

	AudioComponent(Actor* owner) : ActorComponent(owner) {}

	void playSound(const eastl::string& filename, bool bLoop = false, bool bStartPlaying = true);

	void startPlaying(const eastl::string& filename, bool bLoop = false);

	void stopPlaying(const eastl::string& filename, bool bRemove = true);

	void stopPlaying();

	void pausePlaying(const eastl::string& filename);

	void removeSound(const eastl::string& filename);

	sf::Sound* getSound(const eastl::string& filename) const
	{
		if (m_sounds.count(filename))
		{
			return m_sounds.at(filename).ptr.get();
		}
		return nullptr;
	}

	void setOnPlayingFinished(const eastl::string& filename, OnPlayingFinishedSignature event)
	{
		if (m_sounds.count(filename))
		{
			m_sounds[filename].onPlayingFinished = event;
		}
	}

	virtual void tick(float deltaSeconds) override;

private:
	struct SoundData
	{
		eastl::unique_ptr<sf::Sound> ptr = nullptr;
		OnPlayingFinishedSignature onPlayingFinished = nullptr;
		float duration = 0.f;
	};

	eastl::hash_map<eastl::string, SoundData, eastl::hash<eastl::string>, eastl::equal_to<eastl::string>, eastl::allocator, true> m_sounds;

};