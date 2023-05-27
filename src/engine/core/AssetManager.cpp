#include "core/AssetManager.hpp"
#include "components/CharacterAnimComponent.hpp"
#include "actors/Map.hpp"
#include "core/World.hpp"
#include <boost/json.hpp>
#include <fstream>

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
	EASTL_ASSERT(m_animGroups.count("movement"));
	CharacterAnimComponent::setMovementGroup(m_animGroups.at("movement"));
}

Map* AssetManager::loadMap(Map* map, const eastl::string& filename)
{
	namespace json = boost::json;
	std::ifstream ifs(filename.c_str());
	EASTL_ASSERT_MSG(ifs, "Failed to open file");

	json::object mapInfo = json::parse(ifs).as_object();

	json::object cellSizeInfo = mapInfo.at("cellSize").as_object();
	Vector3f cellSize
	(
		cellSizeInfo.at("x").as_int64(),
		cellSizeInfo.at("y").as_int64(),
		cellSizeInfo.at("z").as_int64()
	);
	map->setCellSize(cellSize);

	json::array spawnPointsInfo = mapInfo.at("spawnPoints").as_array();
	for (const auto& i : spawnPointsInfo)
	{
		json::object curSpawnPointInfo = i.as_object();
		Vector3i curSpawnPoint(
			curSpawnPointInfo.at("coordinates").as_object().at("x").as_int64(),
			curSpawnPointInfo.at("coordinates").as_object().at("y").as_int64(),
			curSpawnPointInfo.at("coordinates").as_object().at("z").as_int64()
		);
		map->addSpawnPoint(curSpawnPointInfo.at("id").as_int64(), curSpawnPoint);
	}

	json::array segmentsInfo = mapInfo.at("segments").as_array();
	for (const auto& i : segmentsInfo)
	{
		json::object curSegmentInfo = i.as_object();

		json::string type = curSegmentInfo.at("type").as_string();

		sf::Texture* majorFiller = nullptr;
		if (curSegmentInfo.contains("majorFiller"))
		{
			majorFiller = loadTexture(curSegmentInfo.at("majorFiller").as_string().c_str());
		}

		int32_t layer = -1;
		if (curSegmentInfo.contains("layer"))
		{
			layer = curSegmentInfo.at("layer").as_int64();
		}

		eastl::hash_map<Vector3i, sf::Texture*> minorFillers;

		if (curSegmentInfo.contains("minorFillers"))
		{
			json::array minorFillersInfo = curSegmentInfo.at("minorFillers").as_array();
			for (const auto& i : minorFillersInfo)
			{
				json::object curMinorFillerInfo = i.as_object();
				sf::Texture* texture = nullptr;
				if (curMinorFillerInfo.contains("texture"))
				{
					texture = loadTexture(curMinorFillerInfo.at("texture").as_string().c_str());
				}
				Vector3i lower(
					curMinorFillerInfo.at("lower").as_object().at("x").as_int64(),
					curMinorFillerInfo.at("lower").as_object().at("y").as_int64(),
					curMinorFillerInfo.at("lower").as_object().at("z").as_int64()
				);
				Vector3i upper(
					curMinorFillerInfo.at("upper").as_object().at("x").as_int64(),
					curMinorFillerInfo.at("upper").as_object().at("y").as_int64(),
					curMinorFillerInfo.at("upper").as_object().at("z").as_int64()
				);
				for (int32_t x = lower.x; x <= upper.x; ++x)
				{
					for (int32_t y = lower.y; y <= upper.y; ++y)
					{
						for (int32_t z = lower.z; z <= upper.z; ++z)
						{
							minorFillers[{x, y, z}] = texture;
						}
					}
				}
			}
		}

		if (type == "box")
		{
			Vector3i lower(
				curSegmentInfo.at("lower").as_object().at("x").as_int64(),
				curSegmentInfo.at("lower").as_object().at("y").as_int64(),
				curSegmentInfo.at("lower").as_object().at("z").as_int64()
			);
			Vector3i upper(
				curSegmentInfo.at("upper").as_object().at("x").as_int64(),
				curSegmentInfo.at("upper").as_object().at("y").as_int64(),
				curSegmentInfo.at("upper").as_object().at("z").as_int64()
			);
			map->addFloor(lower, upper, majorFiller, minorFillers, layer);
		}
		else
		{
			Vector3i position(
				curSegmentInfo.at("position").as_object().at("x").as_int64(),
				curSegmentInfo.at("position").as_object().at("y").as_int64(),
				curSegmentInfo.at("position").as_object().at("z").as_int64()
			);
			Vector2i size(
				curSegmentInfo.at("size").as_object().at("x").as_int64(),
				curSegmentInfo.at("size").as_object().at("y").as_int64()
			);
			sf::Texture* sideFiller = nullptr;
			if (curSegmentInfo.contains("sideFiller"))
			{
				sideFiller = loadTexture(curSegmentInfo.at("sideFiller").as_string().c_str());
			}
			if (type == "ramp_x")
			{
				map->addRampX(position, size, sideFiller, majorFiller, minorFillers, layer);
			}
			else if (type == "ramp_y")
			{
				map->addRampY(position, size, sideFiller, majorFiller, minorFillers, layer);
			}
		}
	}
	if (mapInfo.contains("triggers"))
	{
		json::array triggers = mapInfo.at("triggers").as_array();
		for (const auto& i : triggers)
		{
			json::object curTriggetInfo = i.as_object();
			Vector3i lower(
				curTriggetInfo.at("lower").as_object().at("x").as_int64(),
				curTriggetInfo.at("lower").as_object().at("y").as_int64(),
				curTriggetInfo.at("lower").as_object().at("z").as_int64()
			);
			Vector3i upper(
				curTriggetInfo.at("upper").as_object().at("x").as_int64(),
				curTriggetInfo.at("upper").as_object().at("y").as_int64(),
				curTriggetInfo.at("upper").as_object().at("z").as_int64()
			);
			uint8_t tag = curTriggetInfo.at("tag").as_int64();
			map->addTrigger(lower, upper, tag);
		}
	}
	return map;
}