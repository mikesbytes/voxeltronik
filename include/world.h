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

#include "cuckoohash_map.hh"
#include "concurrentqueue.h"

#include <unordered_map>
#include <vector>
#include <deque>
#include <glm/glm.hpp>

namespace vtk {

class Chunk;

class World {
	friend class Chunk;
public:
    World();
    bool isVoxelSolid(const int& x, const int& y, const int& z);

    bool setVoxelType(const int& x, const int& y, const int& z, const unsigned& type, const bool& updateChunk = false);

    unsigned getVoxelType(const glm::ivec3& pos);

	Chunk* makeChunk(const int& x, const int& y, const int& z, bool insertAfter = true);
    bool generateChunk(const int& x, const int& y, const int& z);
	bool insertChunk(Chunk* chunk);

    Chunk* getChunk(const glm::ivec3& pos);

    void queueChunkUpdate(const int& x, const int& y, const int& z, const bool& highpriority = false);
    void queueChunkUpdate(const glm::ivec3& pos, const bool& highpriority = false);

	void queueChunkLoad(const glm::ivec3& pos);

    void draw();
    void update();

    void forceGlobalGeometryUpdate(); //Rebuilds all geometry. Don't do this.

	void queueChunkLoadsAroundPoint(const glm::vec3& point, const int& chunkRadius);

	//std::unordered_map<glm::ivec3, Chunk*, ivec3Hash> mChunks;
	cuckoohash_map<glm::ivec3, Chunk*, ivec3Hash> mChunks;
    cuckoohash_map<glm::ivec3, ChunkMesh*, ivec3Hash> mChunkMeshes;
    std::vector<iPos> chunkUpdateQueue;
    std::deque<glm::ivec3> mChunkUpdateQueue;
	std::deque<glm::ivec3> mChunkLoadQueue;

	// mesh update queues
	moodycamel::ConcurrentQueue<glm::ivec3> mMeshUpdateQueue; // regular one
	moodycamel::ConcurrentQueue<glm::ivec3> mMeshUpdateQueueSoon; // high priority one

    unsigned chunkSize;
    float voxelSize;

    int modelMatUni;

    TerrainGen terrain;
    VoxelInfo voxelInfo;
    VoxelMath voxelMath;
    bool rebuildThreadActive;
	bool mLoadThreadActive;
};

}
