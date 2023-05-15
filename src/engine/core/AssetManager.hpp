#pragma once

#include <EASTL/string.h>
#include <EASTL/hash_map.h>
#include <EASTL/unique_ptr.h>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>

class AssetManager
{
	static eastl::hash_map<eastl::string, eastl::unique_ptr<sf::Texture>, eastl::hash<eastl::string>, eastl::equal_to<eastl::string>, eastl::allocator, true> m_textures;

	static eastl::hash_map<eastl::string, eastl::unique_ptr<sf::SoundBuffer>, eastl::hash<eastl::string>, eastl::equal_to<eastl::string>, eastl::allocator, true> m_sounds;

public:
	AssetManager() = delete;

	~AssetManager() = delete;

	static sf::Texture* loadTexture(const eastl::string& filename);

	static sf::SoundBuffer* loadSound(const eastl::string& filename);
};