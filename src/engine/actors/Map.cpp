#include "actors/Map.hpp"
#include <SFML/Graphics/Texture.hpp>
#include "components/RampComponent.hpp"
#include "components/BoxComponent.hpp"
#include "components/SpriteComponent.hpp"

void Map::addFloor(const Vector3i& lowerCorner, const Vector3i& upperCorner, sf::Texture* majorFiller, const eastl::hash_map<Vector3i, sf::Texture*>& minorFillers, int32_t layer)
{
	Vector3f blockSize((upperCorner.x - lowerCorner.x + 1) * m_cellSize.x, (upperCorner.y - lowerCorner.y + 1) * m_cellSize.y, (upperCorner.z - lowerCorner.z + 1) * m_cellSize.z);
	Vector3f blockPosition(blockSize.x / 2 + lowerCorner.x * m_cellSize.x, blockSize.y / 2 + lowerCorner.y * m_cellSize.y, blockSize.z / 2 + lowerCorner.z * m_cellSize.z);
	BoxComponent* collision = new BoxComponent();
	collision->setSize(blockSize);
	collision->setPosition(blockPosition);
	collision->setOverlapRule(ActorsGroups::MAP, CollisionComponent::OverlapRules::IGNORE);
	collision->attachToActor(this);

	for (int32_t x = lowerCorner.x; x <= upperCorner.x; ++x)
	{
		for (int32_t y = lowerCorner.y; y <= upperCorner.y; ++y)
		{
			for (int32_t z = lowerCorner.z; z <= upperCorner.z; ++z)
			{
				Vector3i currentCell(x, y, z);
				auto blockToInsert = minorFillers.find(currentCell);
				sf::Texture* texture = blockToInsert == minorFillers.end() ? majorFiller : blockToInsert->second;
				if (texture)
				{
					SpriteComponent* blockSprite = new SpriteComponent();
					blockSprite->setLayer(layer);
					blockSprite->setTexture(texture);
					blockSprite->setPosition({ currentCell.x * m_cellSize.x, currentCell.y * m_cellSize.y, currentCell.z * m_cellSize.z });
					blockSprite->attachToActor(this);
					blockSprite->setHeight(m_cellSize.z);
				}
			}
		}
	}
}

void Map::addRampX(const Vector3i& position, const Vector2i& size, sf::Texture* blockFiller, sf::Texture* majorFiller, const eastl::hash_map<Vector3i, sf::Texture*>& minorFillers, int32_t layer)
{
	Vector3f rampSize(size.y * m_cellSize.y, size.x * m_cellSize.x, size.y * m_cellSize.y);
	Vector3f rampPosition(rampSize.y / 2 + position.x * m_cellSize.x, rampSize.x / 2 + position.y * m_cellSize.y, rampSize.z / 2 + position.z * m_cellSize.z);
	RampComponent* ramp = new RampComponent();
	ramp->setSize(rampSize);
	ramp->setPosition(rampPosition);
	ramp->setRotation(mathter::RotationAxis<2>(mathter::Deg2Rad(90)));
	ramp->setOverlapRule(ActorsGroups::MAP, CollisionComponent::OverlapRules::IGNORE);
	ramp->attachToActor(this);

	for (int32_t y = position.y + size.y - 1, z = position.z; y >= position.y; --y, ++z)
	{
		for (int32_t x = position.x; x != position.x + size.x; ++x)
		{
			Vector3i currentCell(x, y, z);
			auto blockToInsert = minorFillers.find(currentCell);
			sf::Texture* texture = blockToInsert == minorFillers.end() ? majorFiller : blockToInsert->second;
			if (texture)
			{
				SpriteComponent* blockSprite = new SpriteComponent();
				blockSprite->setLayer(layer);
				blockSprite->setTexture(texture);
				blockSprite->setPosition({ currentCell.x * m_cellSize.x, currentCell.y * m_cellSize.y, currentCell.z * m_cellSize.z });
				blockSprite->attachToActor(this);
				blockSprite->setHeight(m_cellSize.z);
			}
		}
	}

	for (int32_t z = position.z; z != position.z + size.y; ++z)
	{
		for (int32_t y = position.y; y != position.y + size.y - z - position.z + 1; ++y)
		{
			Vector3i currentCell(position.x + size.x - 1, y, z);
			auto blockToInsert = minorFillers.find(currentCell);
			sf::Texture* texture = blockToInsert == minorFillers.end() ? blockFiller : blockToInsert->second;
			if (texture)
			{
				SpriteComponent* blockSprite = new SpriteComponent();
				blockSprite->setLayer(layer);
				blockSprite->setTexture(texture);
				blockSprite->setPosition({ currentCell.x * m_cellSize.x, currentCell.y * m_cellSize.y, currentCell.z * m_cellSize.z });
				blockSprite->attachToActor(this);
				blockSprite->setHeight(m_cellSize.z);
			}
		}
	}
}
