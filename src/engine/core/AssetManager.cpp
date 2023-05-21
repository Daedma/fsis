#include "core/AssetManager.hpp"
#include "components/CharacterAnimComponent.hpp"
#include <boost/json.hpp>
#include <fstream>
#include "log.hpp"

eastl::hash_map<eastl::string, eastl::unique_ptr<sf::Texture>, eastl::hash<eastl::string>, eastl::equal_to<eastl::string>, eastl::allocator, true> AssetManager::m_textures;

eastl::hash_map<eastl::string, eastl::unique_ptr<sf::SoundBuffer>, eastl::hash<eastl::string>, eastl::equal_to<eastl::string>, eastl::allocator, true> AssetManager::m_sounds;

eastl::hash_map<eastl::string, uint32_t, eastl::hash<eastl::string>, eastl::equal_to<eastl::string>, eastl::allocator, true> AssetManager::m_animGroups;

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

CharacterAnimComponent* AssetManager::loadCharacterAnimation(const eastl::string& filename)
{
	namespace json = boost::json;
	std::ifstream ifs(filename.c_str());
	EASTL_ASSERT_MSG(ifs, "Failed to open file");

	json::object animationInfo = json::parse(ifs).as_object();

	eastl::string file = animationInfo.at("file").as_string().c_str();
	sf::Texture* spriteSheet = loadTexture(file);

	json::object frameInfo = animationInfo.at("frame").as_object();
	sf::Vector2i frameSize(frameInfo.at("width").as_int64(), frameInfo.at("height").as_int64());

	json::object indentInfo = animationInfo.at("indent").as_object();
	sf::Vector2i indent(indentInfo.at("left").as_int64(), indentInfo.at("top").as_int64());

	uint64_t height = animationInfo.at("height").as_int64();

	CharacterAnimComponent* result = new CharacterAnimComponent();

	json::array animations = animationInfo.at("animations").as_array();
	for (const auto& i : animations)
	{
		Animation* currentAnimation = nullptr;
		json::object cur = i.as_object();
		LOG(cur);
		uint32_t group = getAnimationGroupId(cur.at("group").as_string().c_str());
		if (cur.contains("sector"))
		{
			uint8_t sector = cur.at("sector").as_int64();
			currentAnimation = result->addOrientedAnimation(sector, group);
		}
		else
		{
			currentAnimation = result->addUndirectedAnimation(group);
		}
		currentAnimation->setTexture(spriteSheet);

		uint64_t line = cur.at("line").as_int64() - 1;
		sf::IntRect curFrame(indent.x, indent.y + line * frameSize.y, frameSize.x, frameSize.y);
		currentAnimation->setInitFrame(curFrame);

		uint64_t framesCount = cur.at("framesCount").as_int64();
		currentAnimation->setFramesCount(framesCount);

		float frequency = cur.at("frequency").as_int64();
		currentAnimation->setFrequency(frequency);

		bool loop = cur.at("loop").as_bool();
		currentAnimation->setLoop(loop);

		currentAnimation->setHeight(height);
	}
	return result;
}

void AssetManager::initAnimGroups(const eastl::string& filename)
{
	namespace json = boost::json;
	std::ifstream ifs(filename.c_str());
	EASTL_ASSERT_MSG(ifs, "Failed to open file");

	json::array groups = json::parse(ifs).as_object().at("groups").as_array();
	for (const auto& i : groups)
	{
		json::object cur = i.as_object();
		m_animGroups[cur.at("name").as_string().c_str()] = cur.at("id").as_int64();
	}

}

