#pragma once

#include "voxelutils.h"
#include "util/mobileatomic.h"

#include <glm/glm.hpp>
#include <queue>

namespace vtk {

class World;
class HeightMap;

typedef std::pair<short, unsigned short> LightIndexPair;

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
	bool isVoxelSolid(const glm::ivec3& pos);

	void setVoxelType(const glm::ivec3& pos, const unsigned& type);
	void setVoxelType(const int& x, const int& y, const int& z, const unsigned& type, const bool& update = false);

	unsigned getVoxelType(const glm::ivec3& pos);
    unsigned getVoxelType(const unsigned& x, const unsigned& y, const unsigned& z);

	void rebuildLighting();
	void addLight(const glm::ivec3& pos, const unsigned short& light);
	void removeLight(const glm::ivec3& pos);


    glm::ivec3 getWorldCoords(const int& x, const int& y, const int& z);
	unsigned getLightLevel(const glm::ivec3& pos);
	unsigned short getLightPacked(const glm::ivec3& pos);
	void setLightPacked(const glm::ivec3& pos, const unsigned short& light);
	HeightMap* getHeightMap();

	void setPos(const glm::ivec3& pos);
	glm::ivec3 getPos();
	World& getWorld();

	//has no protections for out of bounds
	unsigned short fastGetLightPacked(const glm::ivec3& pos);

protected:

	void propogateLight();
	void propogateLightTask();

	//conversion funcs for directly accessing data and lighting
	short vecToIndex(const glm::ivec3& pos);
	glm::ivec3 indexToVec(const short& index);

	//index, mask, Chunk* tuple
	typedef std::tuple<short, unsigned short, Chunk*> LightIndexTup;

	void setQueuedForMeshRebuild(const bool& rebuild = true);
	bool isQueuedForMeshRebuild();

	std::array<util::MobileAtomic<unsigned>, 4096> mData;
	std::array<util::MobileAtomic<unsigned short>, 4096> mLighting;
    World& mLinkedWorld;
    glm::ivec3 mPos;

	//queue for lighting voxels with BFS
	std::queue<LightIndexTup> mLightBFSQueue;
	//queue for removing lights with BFS
	std::queue<LightIndexTup> mDarkBFSQueue;

	std::atomic<bool> mLoaded;
	std::atomic<bool> mQueuedForMeshRebuild;
	std::atomic<bool> mPropogating;
};

}
