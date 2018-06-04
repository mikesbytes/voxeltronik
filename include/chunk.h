#pragma once

#include "voxelutils.h"
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

	// BREAK AND PLACE FUNCS
	// These handle all the mesh rebuilds and heightmap updates for you
	// Positions are chunk relative (0-15 on all axes)
	
	//break a voxel, returns the type of the voxel that was just broken
	unsigned breakVoxel(const glm::ivec3& pos);
	//place voxel, will not override previous voxels, returns true if successful
	bool placeVoxel(const glm::ivec3& pos, const unsigned& type);
	
    bool isVoxelSolid(const int& x, const int& y, const int& z); //Is the voxel not a transparent type?

	void setVoxelType(const glm::ivec3& pos, const unsigned& type);
	void setVoxelType(const int& x, const int& y, const int& z, const unsigned& type, const bool& update = false);

	unsigned getVoxelType(const glm::ivec3& pos);
    unsigned getVoxelType(const unsigned& x, const unsigned& y, const unsigned& z);

    glm::ivec3 getWorldCoords(const int& x, const int& y, const int& z);
	unsigned getLightLevel(const glm::ivec3& pos);
	unsigned short getLightPacked(const glm::ivec3& pos);
	HeightMap* getHeightMap();

	void setPos(const glm::ivec3& pos);
	glm::ivec3 getPos();
	World& getWorld();


protected:
	void setQueuedForMeshRebuild(const bool& rebuild = true);
	bool isQueuedForMeshRebuild();

	std::array<util::MobileAtomic<unsigned>, 4096> mData;
	std::array<util::MobileAtomic<unsigned short>, 4096> mLighting;
    World& mLinkedWorld;
    glm::ivec3 mPos;
	std::atomic<bool> mLoaded;
	std::atomic<bool> mQueuedForMeshRebuild;
};

}
