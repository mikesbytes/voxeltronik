#include "heightmap.h"
#include "world.h"

namespace vtk {

HeightMap::HeightMap(const glm::ivec2& pos, World& world) :
	mPos(pos),
	mWorld(world)
{
	//initialize all heights to 0
	for (int i = 0; i < mHeights.size(); ++i) {
		mHeights[i].store(0x80000000);
	}
}

glm::ivec2 HeightMap::getPos() {
	return mPos;
}

int HeightMap::getHeight(const glm::ivec2 &pos) {
	int index = pos.x + (16 * pos.y);
	return mHeights[index].load();
}

bool HeightMap::pushUpdate(const glm::ivec3 &pos, const bool &destroyed) {
	return mUpdateQueue.try_enqueue(std::make_pair(pos, destroyed));
}

void HeightMap::blockHeight(const glm::ivec3& pos) {
	if (getHeight(glm::ivec2(pos.x, pos.z)) < pos.y) {
		setHeight(pos);
	}
}

void HeightMap::unblockHeight(const glm::ivec3& pos) {
	int height = getHeight(glm::ivec2(pos.x, pos.z));
	if (height <= pos.y) {
		glm::ivec3 searchPos(pos.x + (mPos.x * 16), pos.y, pos.z + (mPos.y * 16));
		while (!mWorld.isVoxelSolid(searchPos.x, searchPos.y, searchPos.z)) {
			--searchPos.y;
		}
		setHeight(glm::ivec3(pos.x, searchPos.y, pos.z));
	}
}

void HeightMap::flushUpdates() {
	std::pair<glm::ivec3, bool> updateItem;
	while (mUpdateQueue.try_dequeue(updateItem)) {
		glm::ivec3& pos = updateItem.first;
		int height = getHeight(glm::ivec2(pos.x, pos.z));
		if (updateItem.second) { // if destroyed
			if(height == pos.y) { // if voxel is at the top of the heightmap
				// search downward until the next solid voxel is reached
				glm::ivec3 searchPos(pos.x + (mPos.x * 16), pos.y, pos.z + (mPos.y * 16));
				while(!mWorld.isVoxelSolid(searchPos.x, searchPos.y, searchPos.z)) {
					--searchPos.y;
				}
				setHeight(glm::ivec3(pos.x, searchPos.y, pos.z));
			}
		} else { // if placed
			if (height < pos.y)
				setHeight(pos);
		}
	}
}

void HeightMap::setHeight(const glm::ivec3& pos) {
	mHeights[pos.x + (16 * pos.z)].store(pos.y);
}

}
