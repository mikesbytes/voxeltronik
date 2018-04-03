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
#include "graphics/chunkmesh.h"

#include <glm/glm.hpp>

namespace vtk {

class World;

class Chunk {
public:
    Chunk(World& world);
	bool isLoaded();

    bool isVoxelSolid(const int& x, const int& y, const int& z); //Is the voxel not a transparent type?
	void setVoxelType(const int& x, const int& y, const int& z, const unsigned& type, const bool& update = false);
    unsigned getVoxelType(const unsigned& x, const unsigned& y, const unsigned& z);
    glm::ivec3 getWorldCoords(const int& x, const int& y, const int& z);

	void setPos(const glm::ivec3& pos);
	glm::ivec3 getPos();
	World& getWorld();


    ChunkRenderer renderer;
protected:
    std::array<unsigned, 4096> voxels;
    World& mLinkedWorld;
    glm::ivec3 mPos;
	std::atomic<bool> mLoaded;
};

}
