#include "world.h"
#include "chunk.h"
#include "threadpool.h"

#include <cmath>
#include <iostream>
#include <thread>

#include <GL/glew.h>
#include <GL/gl.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace vtk {

World::World() {
	chunkSize = 16;
	voxelSize = 1.0f;
	voxelInfo.linkedWorld = this;
	voxelMath.linkedWorld = this;
	rebuildThreadActive = false;
	mLoadThreadActive = false;
}

bool World::isVoxelSolid(const int& x, const int& y, const int& z) {
	auto chunkPos = glm::ivec3(floor((float)x / (float)chunkSize),
	                           floor((float)y / (float)chunkSize),
	                           floor((float)z / (float)chunkSize));
	if (mChunks.find(chunkPos) == mChunks.end()) { //block is in nonexistant chunk
		return true;
	}

	int relPosX = x - chunkPos.x * chunkSize;
	int relPosY = y - chunkPos.y * chunkSize;
	int relPosZ = z - chunkPos.z * chunkSize;

	if (!mChunks[chunkPos]->isLoaded()) return true;
	return mChunks[chunkPos]->isVoxelSolid(relPosX, relPosY, relPosZ);
}

bool World::setVoxelType(const int& x, const int& y, const int& z, const unsigned& type, const bool& updateChunk) {
	auto chunkPos = glm::ivec3(floor((float)x / (float)chunkSize), floor((float)y / (float)chunkSize), floor((float)z / (float)chunkSize));
	if (mChunks.find(chunkPos) == mChunks.end()) { //block is in nonexistant chunk
		return false;
	}

	int relPosX = x - chunkPos.x * chunkSize;
	int relPosY = y - chunkPos.y * chunkSize;
	int relPosZ = z - chunkPos.z * chunkSize;

	mChunks[chunkPos]->setVoxelType(relPosX, relPosY, relPosZ, type);
	if (updateChunk) queueChunkUpdate(chunkPos, true);
	return true;
}

unsigned World::getVoxelType(const glm::ivec3& pos) {
	auto chunkPos = glm::ivec3(floor((float)pos.x / (float)chunkSize),
	                           floor((float)pos.y / (float)chunkSize),
	                           floor((float)pos.z / (float)chunkSize));
	if (mChunks.find(chunkPos) == mChunks.end()) {
		return 0;
	}

	int relPosX = pos.x - chunkPos.x * chunkSize;
	int relPosY = pos.y - chunkPos.y * chunkSize;
	int relPosZ = pos.z - chunkPos.z * chunkSize;

	return mChunks[chunkPos]->getVoxelType((unsigned)relPosX, (unsigned)relPosY, (unsigned)relPosZ);
}


Chunk* World::makeChunk(const int& x, const int& y, const int& z, bool insertAfter) {
	auto pos = glm::ivec3(x, y, z);

	if (mChunks.find(pos) != mChunks.end()) { //chunk already exists
		return mChunks[pos]; 
	}

	auto newChunk = new Chunk(*this);
	newChunk->setPos(pos);

	if (insertAfter) {
		mChunks[pos] = newChunk;
		mChunkMeshes.emplace(pos, ChunkMesh(*this, pos));
	}

	return newChunk;
}

bool World::generateChunk(const int& x, const int& y, const int& z) {
	auto chunkMade = makeChunk(x,y,z, false);
	if (chunkMade != nullptr) {
		terrain.generateChunk(chunkMade);
		//queue this chunk for geometry update
		queueChunkUpdate(x,y,z);

		//queue the neighboring 6 chunks. the queue functions throws them out
		// if they don't exist
		queueChunkUpdate(x+1,y,z);
		queueChunkUpdate(x-1,y,z);
		queueChunkUpdate(x,y+1,z);
		queueChunkUpdate(x,y-1,z);
		queueChunkUpdate(x,y,z+1);
		queueChunkUpdate(x,y,z-1);
	}
	insertChunk(chunkMade);
	return chunkMade;
}

bool World::insertChunk(Chunk* chunk) {
	mChunks[chunk->getPos()] = chunk;
	mChunkMeshes.emplace(chunk->getPos(), ChunkMesh(*this, chunk->getPos()));
	return true;
}


Chunk* World::getChunk(const glm::ivec3& pos) {
	auto chunk = mChunks.find(pos)->second;
	if (chunk) return chunk;
	else return nullptr;
}

void World::queueChunkUpdate(const int& x, const int& y, const int& z, const bool& back) {
	queueChunkUpdate(glm::ivec3(x,y,z));
}

void World::queueChunkUpdate(const glm::ivec3& pos, const bool& back) {
	if (mChunks.find(pos) == mChunks.end()) return; //chunk doesn't exist
	for (auto& i : mChunkUpdateQueue) {
		if (i == pos) return; //chunk is already in queue, we don't need to update multiple times
	}

	if (back) {
		mChunkUpdateQueue.push_back(pos);
	} else {
		mChunkUpdateQueue.push_front(pos);
	}
}

void World::queueChunkLoad(const glm::ivec3 &pos) {
	if (mChunks.find(pos) != mChunks.end()) return; //chunk already exists
	for (auto& i : mChunkLoadQueue) {
		if (i == pos) return;
	}

	makeChunk(pos.x, pos.y, pos.z);
	mChunkLoadQueue.push_back(pos);
}

void World::draw() {
	for (auto& i : mChunkMeshes) {
		glm::mat4 modelMat = glm::translate(glm::mat4(), glm::vec3((float)i.first.x * 16,
		                                                           (float)i.first.y * 16,
		                                                           (float)i.first.z * 16
		                                                           ));

		glUniformMatrix4fv(modelMatUni, 1, GL_FALSE, glm::value_ptr(modelMat));
		i.second.draw();
	}
}

void World::update() {
	if (!rebuildThreadActive && !mChunkUpdateQueue.empty( )) {
		auto updatefunc = [&]() {
			while (!mChunkUpdateQueue.empty()) {
				auto& pos = mChunkUpdateQueue.back();
				auto& mesh = mChunkMeshes.find(pos)->second;
				mChunkUpdateQueue.pop_back();
				if (!mesh.rebuildChunkGeometry())
					queueChunkUpdate(pos, false);
			}
			rebuildThreadActive = false;
		};

		rebuildThreadActive = true;
		ThreadPool::getInstance().addJob(updatefunc);
	}

	if (!mLoadThreadActive && !mChunkLoadQueue.empty()) {
		auto loadFunc = [&]() {
			while (!mChunkLoadQueue.empty()) {
				auto& pos = mChunkLoadQueue.back();
				generateChunk(pos.x, pos.y, pos.z);
				mChunkLoadQueue.pop_back();
			}
			mLoadThreadActive = false;
		};

		mLoadThreadActive = true;
		ThreadPool::getInstance().addJob(loadFunc);
	}
}

void World::forceGlobalGeometryUpdate() {
	int chunkCount = 1;
	chunkCount = 1;
	for (auto& i : mChunkMeshes) {
		std::cout << "\rUpdating chunk geometry, but better (" << chunkCount << "/" << mChunkMeshes.size() << ")" << std::flush;
		i.second.rebuildChunkGeometry();
		i.second.updateGeometry();
		++chunkCount;
	}
}

void World::queueChunkLoadsAroundPoint(const glm::vec3 &point, const int &chunkRadius) {
	glm::ivec3 chunkPoint = point / 16.0f;
	for (int z = -chunkRadius; z <= chunkRadius; ++z) {
		for (int x = -chunkRadius; x <= chunkRadius; ++x) {
			if (z*z + x*x <= chunkRadius * chunkRadius) {
				for (int y = 0; y < 8; ++y) {
					queueChunkLoad(glm::ivec3(x + chunkPoint.x, y, z + chunkPoint.z));
				}
			}
		}
	}
}

}

