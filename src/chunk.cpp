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
	mLinkedWorld(world)
{
    mPos = glm::ivec3(0,0,0);
    renderer.linkedChunk = this;

    //fill voxels with 0
    for (unsigned i = 0; i < 4096; i++) {
        voxels[i] = 0;
    }
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

    return (getVoxelType((unsigned)x,(unsigned)y,(unsigned)z) != 0);
}

void Chunk::setVoxelType(const int& x, const int& y, const int& z, const unsigned& type) {
    auto index = x + 16 * (y + 16 * z);
    if (index > 4095) {
        std::cout << "CHUNK ACCESS ERROR (set voxel): Out of range, doing nothing\n";
        return;
    }
    voxels[index] = type;

}

unsigned Chunk::getVoxelType(const unsigned& x, const unsigned& y, const unsigned& z) {
    auto index = x + 16 * (y + 16 * z);
    if (index > 4095) {
        std::cout << "CHUNK ACCESS ERROR (get voxel): Out of range, returning type 0\n";
        return 0;
    }
    return voxels[index];
}

glm::ivec3 Chunk::getWorldCoords(const int& x, const int& y, const int& z) {
    return glm::ivec3(mPos.x * 16 + x,
                      mPos.y * 16 + y,
                      mPos.z * 16 + z);
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

}
