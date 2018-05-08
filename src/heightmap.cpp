#include "heightmap.h"

namespace vtk {

HeightMap::HeightMap(const glm::ivec2& pos) :
	mPos(pos)
{
	
}

glm::ivec2 HeightMap::getPos() {
	return mPos;
}

int HeightMap::getHeight(const glm::ivec2 &pos) {
	int index = pos.x + (16 * pos.y);
	return mHeights[index];
}

bool HeightMap::pushUpdate(const glm::ivec3 &pos, const bool &destroyed) {
	return mUpdateQueue.try_enqueue(pos);
}

void HeightMap::flushUpdates() {
	glm::ivec3 pos;
	while (mUpdateQueue.try_dequeue(pos)) {
		int index = pos.x + (16 * pos.y);
		mHeights[index] = pos.z;
	}
}

}
