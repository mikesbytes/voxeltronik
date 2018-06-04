#include "chunk.h"
#include "world.h"

#include <iostream>

namespace vtk {

Chunk::Chunk(World& world) :
	mLinkedWorld(world),
	mLoaded(false),
	mQueuedForMeshRebuild(false)
{
    mPos = glm::ivec3(0,0,0);

    //fill voxels with 0
    for (unsigned i = 0; i < mData.size(); ++i) {
	    mData[i].store(0);
    }
    for (unsigned i = 0; i < mLighting.size(); ++i) {
	    mLighting[i].store(0);
    }

    mLoaded.store(true);
}

bool Chunk::isLoaded() {
	return mLoaded.load();
}

unsigned Chunk::breakVoxel(const glm::ivec3& pos) {
	unsigned voxelType = getVoxelType(pos);
	if (voxelType == 0) return voxelType; //return early if nothing broken

	setVoxelType(pos, 0);

	//update heightmap
	mLinkedWorld.getHeightMap(glm::ivec2(mPos.x, mPos.z))
		->unblockHeight(glm::ivec3(pos.x, pos.y + mPos.y * 16, pos.z));
	mLinkedWorld.queueChunkUpdate(mPos);

	//update the neightboring chunks if voxel lies along border
	glm::ivec3 neighborPos(0,0,0);

	if (pos.x == 0) neighborPos.x--;
	else if (pos.x == 15) neighborPos.x++;

	if (pos.y == 0) neighborPos.y--;
	else if (pos.y == 15) neighborPos.y++;

	if (pos.z == 0) neighborPos.z--;
	else if (pos.z == 15) neighborPos.z++;

	if (neighborPos.x != 0)
		mLinkedWorld.queueChunkUpdate(glm::ivec3(mPos.x + neighborPos.x,
		                                         mPos.y,
		                                         mPos.z));
	if (neighborPos.y != 0)
		mLinkedWorld.queueChunkUpdate(glm::ivec3(mPos.x,
		                                         mPos.y + neighborPos.y,
		                                         mPos.z));
	if (neighborPos.z != 0)
		mLinkedWorld.queueChunkUpdate(glm::ivec3(mPos.x,
		                                         mPos.y,
		                                         mPos.z + neighborPos.z));

	return voxelType;
}

bool Chunk::placeVoxel(const glm::ivec3& pos, const unsigned& type) {
	unsigned curType = getVoxelType(pos);
	if (curType != 0) // return false if the voxel is NOT air
		return false;
	setVoxelType(pos, type);

	//update heightmap
	mLinkedWorld.getHeightMap(glm::ivec2(mPos.x, mPos.z))
		->blockHeight(glm::ivec3(pos.x, pos.y + mPos.y * 16, pos.z));

	mLinkedWorld.queueChunkUpdate(mPos);


	//update the neightboring chunks if voxel lies along border
	glm::ivec3 neighborPos(0,0,0);

	if (pos.x == 0) neighborPos.x--;
	else if (pos.x == 15) neighborPos.x++;

	if (pos.y == 0) neighborPos.y--;
	else if (pos.y == 15) neighborPos.y++;

	if (pos.z == 0) neighborPos.z--;
	else if (pos.z == 15) neighborPos.z++;
	
	if (neighborPos.x != 0)
		mLinkedWorld.queueChunkUpdate(glm::ivec3(mPos.x + neighborPos.x,
		                                         mPos.y,
		                                         mPos.z));
	if (neighborPos.y != 0)
		mLinkedWorld.queueChunkUpdate(glm::ivec3(mPos.x,
		                                         mPos.y + neighborPos.y,
		                                         mPos.z));
	if (neighborPos.z != 0)
		mLinkedWorld.queueChunkUpdate(glm::ivec3(mPos.x,
		                                         mPos.y,
		                                         mPos.z + neighborPos.z));

	return true;
}

bool Chunk::isVoxelSolid(const int& x, const int& y, const int& z) {
    if (x < 0 || x > 15 ||
        y < 0 || y > 15 ||
        z < 0 || z > 15 ) 
	{ //position is outside of the chunk
		return mLinkedWorld.isVoxelSolid(mPos.x * 16 + x,
                                         mPos.y * 16 + y,
                                         mPos.z * 16 + z);
    }

    return !mLinkedWorld.voxelInfo.isTransparent(getVoxelType((unsigned)x, (unsigned)y, (unsigned)z));
}

void Chunk::setVoxelType(const glm::ivec3 &pos, const unsigned& type) {
	setVoxelType(pos.x, pos.y, pos.z, type);
}

void Chunk::setVoxelType(const int& x, const int& y, const int& z, const unsigned& type, const bool& update) {
    auto index = x + 16 * (y + 16 * z);
    if (index > 4095) {
        std::cout << "CHUNK ACCESS ERROR (set voxel): Out of range, doing nothing\n";
        return;
    }
    mData[index].store(type, std::memory_order_release);

    if (!update) return;
    mLinkedWorld.queueChunkUpdate(mPos);
}

unsigned Chunk::getVoxelType(const glm::ivec3& pos) {
    auto index = pos.x + 16 * (pos.y + 16 * pos.z);
    if (index > 4095) {
        std::cout << "CHUNK ACCESS ERROR (get voxel): Out of range, returning type 0\n";
        return 0;
    }
    return mData[index].load(std::memory_order_consume);
}

unsigned Chunk::getVoxelType(const unsigned& x, const unsigned& y, const unsigned& z) {
	return getVoxelType(glm::ivec3(x,y,z));
}
glm::ivec3 Chunk::getWorldCoords(const int& x, const int& y, const int& z) {
    return glm::ivec3(mPos.x * 16 + x,
                      mPos.y * 16 + y,
                      mPos.z * 16 + z);
}

unsigned Chunk::getLightLevel(const glm::ivec3 &pos) {
	if (isVoxelSolid(pos.x, pos.y, pos.z)) {
		return 0;
	}
	return 15;
}

unsigned short Chunk::getLightPacked(const glm::ivec3 &pos) {
	if (isVoxelSolid(pos.x, pos.y, pos.z)) {
		return 0x0000;
	}
	auto wPos = chunkPosToWorldPos(mPos, pos);
	if (mLinkedWorld.getHeight(glm::ivec2(wPos.x, wPos.z)) > wPos.y) { //voxel is below ground
		//std::cout << mLinkedWorld.getHeight(glm::ivec2(pos.x, pos.z)) << ", ";
		return 0x000A;
	}
	return 0x000F;
}

HeightMap* Chunk::getHeightMap() {
	return mLinkedWorld.getHeightMap(glm::ivec2(mPos.x,mPos.z));
}

void Chunk::setPos(const glm::ivec3& pos) {
	mPos = pos;
}


glm::ivec3 Chunk::getPos() {
	return mPos;
}

World& Chunk::getWorld() {
	return mLinkedWorld;
}

void Chunk::setQueuedForMeshRebuild(const bool& rebuild) {
	mQueuedForMeshRebuild = rebuild;
}

bool Chunk::isQueuedForMeshRebuild() {
	return mQueuedForMeshRebuild.load();
}

}
