#include "world.h"
#include "chunk.h"

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


bool World::makeChunk(const int& x, const int& y, const int& z) {
	auto pos = glm::ivec3(x, y, z);

	if (mChunks.find(pos) != mChunks.end()) { //chunk already exists
		return false;
	}

	auto newChunk = new Chunk(*this);
	mChunks[pos] = newChunk;

	newChunk->setPos(pos);

	mChunkMeshes.emplace(pos, ChunkMesh(*this, pos));

	return true;
}

bool World::generateChunk(const int& x, const int& y, const int& z) {
	bool chunkMade = makeChunk(x,y,z);
	if (chunkMade) {
		terrain.generateChunk(getChunk(glm::ivec3(x,y,z)));
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
	return chunkMade;
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
				mesh.rebuildChunkGeometry();
			}
			rebuildThreadActive = false;
		};

		rebuildThreadActive = true;
		std::thread(updatefunc).detach();

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

}
