#include "voxelutils.h"
#include "mathplus.h"

namespace vtk {

glm::ivec3 worldPosToChunkPos(const glm::ivec3& pos) {
	return glm::ivec3(mp::floor_div3(pos.x, 16),
	                  mp::floor_div3(pos.y, 16),
	                  mp::floor_div3(pos.z, 16));
}

glm::ivec2 worldPosToChunkPos(const glm::ivec2& pos) {
	return glm::ivec2(mp::floor_div3(pos.x, 16),
	                  mp::floor_div3(pos.y, 16));
}


glm::ivec3 chunkPosToWorldPos(const glm::ivec3& cPos, const glm::ivec3& lPos) {
	return glm::ivec3((cPos * 16) + lPos);
}
}
