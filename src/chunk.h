/*
 * =====================================================================================
 *
 *       Filename:  chunk.h
 *
 *    Description:  Stores chunk information
 *
 *        Version:  1.0
 *        Created:  04/03/2014 07:23:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include "voxelutils.h"
#include "chunkrenderer.h"

#include <unordered_map>
#include <glm/glm.hpp>

namespace vtk {

class World;

class Chunk {
public:
    Chunk();
    bool isVoxelSolid(const int& x, const int& y, const int& z); //Is the voxel not a transparent type?
    void setVoxelType(const int& x, const int& y, const int& z, const unsigned& type);
    unsigned getVoxelType(const unsigned& x, const unsigned& y, const unsigned& z);
    glm::ivec3 getWorldCoords(const int& x, const int& y, const int& z);

    std::array<unsigned, 4096> voxels;
    World* linkedWorld;
    unsigned chunkSize;
    glm::ivec3 chunkPos;
    ChunkRenderer renderer;
};

}
