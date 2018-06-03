/*
 * =====================================================================================
 *
 *       Filename:  chunk.cpp
 *
 *    Description:  Chunk
 *
 *        Version:  1.0
 *        Created:  04/04/2014 09:44:36 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
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
    renderer.linkedChunk = this;

    //fill voxels with 0
    for (unsigned i = 0; i < mData.size(); i++) {
	    mData[i].store(0);
    }
    mLoaded.store(true);
}

bool Chunk::isLoaded() {
	return mLoaded.load();
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

unsigned Chunk::getVoxelType(const unsigned& x, const unsigned& y, const unsigned& z) {
    auto index = x + 16 * (y + 16 * z);
    if (index > 4095) {
        std::cout << "CHUNK ACCESS ERROR (get voxel): Out of range, returning type 0\n";
        return 0;
    }
    return mData[index].load(std::memory_order_consume);
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

unsigned Chunk::getLightPacked(const glm::ivec3 &pos) {
	if (isVoxelSolid(pos.x, pos.y, pos.z)) {
		return 0x00000000;
	}
	auto wPos = chunkPosToWorldPos(mPos, pos);
	if (mLinkedWorld.getHeight(glm::ivec2(wPos.x, wPos.z)) > wPos.y) { //voxel is below ground
		//std::cout << mLinkedWorld.getHeight(glm::ivec2(pos.x, pos.z)) << ", ";
		return 0x000000AA;
	}
	return 0x000000FF;
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
