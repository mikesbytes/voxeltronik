/*
 * =====================================================================================
 *
 *       Filename:  world.h
 *
 *    Description:  World manages and draws chunks
 *
 *        Version:  1.0
 *        Created:  04/03/2014 09:48:12 PM
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
#include "voxelinfo.h"
#include "voxelmath.h"
#include "terraingen.h"
#include "graphics/chunkmesh.h"

#include <unordered_map>
#include <vector>
#include <deque>
#include <glm/glm.hpp>

namespace vtk {

class Chunk;

class World {
public:
    World();
    bool isVoxelSolid(const int& x, const int& y, const int& z);

    bool setVoxelType(const int& x, const int& y, const int& z, const unsigned& type, const bool& updateChunk = false);

    unsigned getVoxelType(const glm::ivec3& pos);

    bool makeChunk(const int& x, const int& y, const int& z);
    bool generateChunk(const int& x, const int& y, const int& z);

    Chunk* getChunk(const glm::ivec3& pos);

    void queueChunkUpdate(const int& x, const int& y, const int& z, const bool& back = false);
    void queueChunkUpdate(const glm::ivec3& pos, const bool& back = false);

    void draw();
    void update();

    void forceGlobalGeometryUpdate(); //Rebuilds all geometry. Don't do this.

    std::unordered_map<glm::ivec3, Chunk*, ivec3Hash> mChunks;
    std::unordered_map<glm::ivec3, ChunkMesh, ivec3Hash> mChunkMeshes;
    std::vector<iPos> chunkUpdateQueue;
    std::deque<glm::ivec3> mChunkUpdateQueue;
    std::vector<glm::ivec3> mChunkGeometryUpdateQueue;

    unsigned chunkSize;
    float voxelSize;

    int modelMatUni;

    TerrainGen terrain;
    VoxelInfo voxelInfo;
    VoxelMath voxelMath;
    bool rebuildThreadActive;
};

}
