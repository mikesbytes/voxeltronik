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

Chunk::Chunk() {
    linkedWorld = nullptr;
    chunkSize = 16;
    chunkPos = glm::ivec3(0,0,0);
    renderer.linkedChunk = this;
    //fill voxels with 0
    for (unsigned i = 0; i < 4096; i++) {
        voxels[i] = 0;
    }
}

bool Chunk::isVoxelSolid(const int& x, const int& y, const int& z) {
    if (x < 0 || x > (int)chunkSize - 1 ||
        y < 0 || y > (int)chunkSize - 1 ||
        z < 0 || z > (int)chunkSize - 1 ) { //position is outside of the chunk
        return linkedWorld->isVoxelSolid(chunkPos.x * 16 + x,
                                         chunkPos.y * 16 + y,
                                         chunkPos.z * 16 + z);
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
    return glm::ivec3(chunkPos.x * 16 + x,
                      chunkPos.y * 16 + y,
                      chunkPos.z * 16 + z);
}

}
