#include "actors/Map.hpp"
#include <SFML/Graphics/Texture.hpp>
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