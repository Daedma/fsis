#include "core/AssetManager.hpp"

eastl::hash_map<eastl::string, eastl::unique_ptr<sf::Texture>, eastl::hash<eastl::string>, eastl::equal_to<eastl::string>, eastl::allocator, true> AssetManager::m_textures;

eastl::hash_map<eastl::string, eastl::unique_ptr<sf::SoundBuffer>, eastl::hash<eastl::string>, eastl::equal_to<eastl::string>, eastl::allocator, true> AssetManager::m_sounds;

sf::Texture* AssetManager::loadTexture(const eastl::string& filename)
{
	if (m_textures.count(filename) == 0)
	{
		m_textures[filename].reset(new sf::Texture());
		m_textures[filename]->loadFromFile(filename.c_str());
	}
	return m_textures[filename].get();
}

sf::SoundBuffer* AssetManager::loadSound(const eastl::string& filename)
{
	if (m_sounds.count(filename) == 0)
	{
		m_sounds[filename].reset(new sf::SoundBuffer());
		m_sounds[filename]->loadFromFile(filename.c_str());
	}
	return m_sounds[filename].get();
}