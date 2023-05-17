#include "components/AudioComponent.hpp"
#include "core/AssetManager.hpp"
#include <SFML/Audio/Sound.hpp>

void AudioComponent::playSound(const eastl::string& filename, bool bLoop, bool bStartPlaying)
{
	sf::SoundBuffer* buff = AssetManager::loadSound(filename);
	SoundData& sound = m_sounds[filename];
	sound.ptr.reset(new sf::Sound());
	sound.ptr->setBuffer(*buff);
	sound.ptr->setLoop(bLoop);
	if (bStartPlaying)
	{
		sound.ptr->play();
	}
}

void AudioComponent::startPlaying(const eastl::string& filename, bool bLoop)
{
	if (m_sounds.count(filename))
	{
		m_sounds[filename].ptr->setLoop(bLoop);
		m_sounds[filename].ptr->play();
	}
	else
	{
		playSound(filename, true);
	}
}

void AudioComponent::stopPlaying(const eastl::string& filename, bool bRemove)
{
	if (m_sounds.count(filename))
	{
		SoundData& sound = m_sounds[filename];
		sound.ptr->stop();
		if (bRemove)
		{
			m_sounds.erase(filename);
		}
	}
}

void AudioComponent::stopPlaying()
{
	for (auto& [filename, sound] : m_sounds)
	{
		sound.ptr->stop();
	}
}

void AudioComponent::pausePlaying(const eastl::string& filename)
{
	if (m_sounds.count(filename))
	{
		m_sounds[filename].ptr->pause();
	}
}

void AudioComponent::removeSound(const eastl::string& filename)
{
	m_sounds.erase(filename);
}

void AudioComponent::tick(float deltaSeconds)
{
	ActorComponent::tick(deltaSeconds);
	for (auto& [filename, sound] : m_sounds)
	{
		switch (sound.ptr->getStatus())
		{
		case sf::Sound::Status::Paused:
			break;
		case sf::Sound::Status::Stopped:
			if (sound.duration != 0 && sound.onPlayingFinished)
			{
				sound.onPlayingFinished(sound.duration);
			}
			sound.duration = 0;
			break;
		case sf::Sound::Status::Playing:
			sound.duration += deltaSeconds;
			break;
		default:
			break;
		}
	}
}