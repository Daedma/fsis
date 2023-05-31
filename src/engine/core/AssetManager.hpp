#pragma once

#include <EASTL/string.h>
#include <EASTL/hash_map.h>
#include <EASTL/unique_ptr.h>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <TGUI/Texture.hpp>
#include <SFML/Audio/Music.hpp>

class AssetManager
{
	static eastl::hash_map<eastl::string, eastl::unique_ptr<sf::Texture>, eastl::hash<eastl::string>, eastl::equal_to<eastl::string>, eastl::allocator, true> m_textures;

	static eastl::hash_map<eastl::string, eastl::unique_ptr<sf::SoundBuffer>, eastl::hash<eastl::string>, eastl::equal_to<eastl::string>, eastl::allocator, true> m_sounds;

	static eastl::hash_map<eastl::string, eastl::unique_ptr<sf::Music>, eastl::hash<eastl::string>, eastl::equal_to<eastl::string>, eastl::allocator, true> m_music;

	static eastl::hash_map<eastl::string, uint32_t, eastl::hash<eastl::string>, eastl::equal_to<eastl::string>, eastl::allocator, true> m_animGroups;

	static eastl::hash_map<eastl::string, eastl::unique_ptr<tgui::Texture>, eastl::hash<eastl::string>, eastl::equal_to<eastl::string>, eastl::allocator, true> m_tguiTextures;

	// for loadSound
	static eastl::string m_soundPrefix;

	// for playMusic
	static eastl::string m_musicPrefix;

	// for loadTexture
	static eastl::string m_texturesPrefix;

	// for loadCharacterAnimation
	static eastl::string m_characterPrefix;

	// for loadMap
	static eastl::string m_mapPrefix;
public:
	AssetManager() = delete;

	~AssetManager() = delete;

	static sf::Texture* loadTexture(const eastl::string& filename);

	static sf::Texture* loadTextureSFML(const eastl::string& filename)
	{
		return loadTexture(filename);
	}

	static tgui::Texture* loadTextureTGUI(const eastl::string& filename);

	static sf::SoundBuffer* loadSound(const eastl::string& filename);

	static sf::Music* playMusic(const eastl::string& filename);

	static sf::Music* stopMusic(const eastl::string& filename);

	static void stopMusic();

	static class CharacterAnimComponent* loadCharacterAnimation(const eastl::string& filename);

	static class Map* loadMap(class Map* map, const eastl::string& filename);

	static void init(const eastl::string& filename);

	static void initAnimGroups(const eastl::string& filename);

	static uint32_t getAnimationGroupId(const eastl::string& groupname)
	{
		return m_animGroups.at(groupname);
	}
};