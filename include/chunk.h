#pragma once

#include "voxelutils.h"
#include "chunkrenderer.h"
#include "graphics/chunkmesh.h"
#include "util/mobileatomic.h"

#include <glm/glm.hpp>

namespace vtk {

class World;
class HeightMap;

class Chunk {
	friend class World;
public:
    Chunk(World& world);
	bool isLoaded();

    bool isVoxelSolid(const int& x, const int& y, const int& z); //Is the voxel not a transparent type?
	void setVoxelType(const int& x, const int& y, const int& z, const unsigned& type, const bool& update = false);
    unsigned getVoxelType(const unsigned& x, const unsigned& y, const unsigned& z);
    glm::ivec3 getWorldCoords(const int& x, const int& y, const int& z);
	unsigned getLightLevel(const glm::ivec3& pos);
	unsigned getLightPacked(const glm::ivec3& pos);
	HeightMap* getHeightMap();

	void setPos(const glm::ivec3& pos);
	glm::ivec3 getPos();
	World& getWorld();


    ChunkRenderer renderer;
protected:
	void setQueuedForMeshRebuild(const bool& rebuild = true);
	bool isQueuedForMeshRebuild();

	std::array<util::MobileAtomic<unsigned>, 4096> mData;
	//std::array<unsigned, 4096> voxels;
    World& mLinkedWorld;
    glm::ivec3 mPos;
	std::atomic<bool> mLoaded;
	std::atomic<bool> mQueuedForMeshRebuild;
};

}
