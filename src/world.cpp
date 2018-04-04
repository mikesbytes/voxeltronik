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
	auto chunk = getChunk(chunkPos);
	if (!chunk) //block is in nonexistant chunk
		return true;

	int relPosX = x - chunkPos.x * chunkSize;
	int relPosY = y - chunkPos.y * chunkSize;
	int relPosZ = z - chunkPos.z * chunkSize;

	if (!chunk->isLoaded()) return true;
	return chunk->isVoxelSolid(relPosX, relPosY, relPosZ);
}

bool World::setVoxelType(const int& x, const int& y, const int& z, const unsigned& type, const bool& updateChunk) {
	auto chunkPos = glm::ivec3(floor((float)x / (float)chunkSize), floor((float)y / (float)chunkSize), floor((float)z / (float)chunkSize));
	auto chunk = getChunk(chunkPos);
	if (!chunk) return false;

	int relPosX = x - chunkPos.x * chunkSize;
	int relPosY = y - chunkPos.y * chunkSize;
	int relPosZ = z - chunkPos.z * chunkSize;

	chunk->setVoxelType(relPosX, relPosY, relPosZ, type, updateChunk);
	return true;
}

unsigned World::getVoxelType(const glm::ivec3& pos) {
	auto chunkPos = glm::ivec3(floor((float)pos.x / (float)chunkSize),
	                           floor((float)pos.y / (float)chunkSize),
	                           floor((float)pos.z / (float)chunkSize));
	Chunk* chunk = getChunk(chunkPos);
	if (!chunk) return 0;
	
	int relPosX = pos.x - chunkPos.x * chunkSize;
	int relPosY = pos.y - chunkPos.y * chunkSize;
	int relPosZ = pos.z - chunkPos.z * chunkSize;

	return chunk->getVoxelType((unsigned)relPosX, (unsigned)relPosY, (unsigned)relPosZ);
}


Chunk* World::makeChunk(const int& x, const int& y, const int& z, bool insertAfter) {
	auto pos = glm::ivec3(x, y, z);

	Chunk* chunk;
	if (mChunks.find(pos, chunk)) { //chunk already exists
		return chunk; 
	}

	chunk = new Chunk(*this);
	chunk->setPos(pos);

	if (insertAfter) insertChunk(chunk);
	return chunk;
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
	mChunks.insert(chunk->getPos(), chunk);
	mChunkMeshes.insert(chunk->getPos(), new ChunkMesh(*this, chunk->getPos()));
	return true;
}


Chunk* World::getChunk(const glm::ivec3& pos) {
	Chunk* chunk;
	if (mChunks.find(pos, chunk) && chunk->isLoaded()) return chunk;
	else return nullptr;
}

void World::queueChunkUpdate(const int& x, const int& y, const int& z, const bool& highpriority) {
	queueChunkUpdate(glm::ivec3(x,y,z));
}

void World::queueChunkUpdate(const glm::ivec3& pos, const bool& highpriority) {
	auto chunk = getChunk(pos);
	if (!chunk || chunk->isQueuedForMeshRebuild()) return;
	chunk->setQueuedForMeshRebuild(true);

	if (highpriority) { // queue for higher rebuild priority
		mMeshUpdateQueueSoon.enqueue(pos);
		return;
	}
	mMeshUpdateQueue.enqueue(pos);
}

void World::queueChunkLoad(const glm::ivec3 &pos) {
	if (getChunk(pos)) return; //chunk already exists
	for (auto& i : mChunkLoadQueue) {
		if (i == pos) return;
	}

	makeChunk(pos.x, pos.y, pos.z);
	mChunkLoadQueue.push_back(pos);
}

void World::draw() {
	auto lt = mChunkMeshes.lock_table();
	for (const auto& i : lt) {
		glm::mat4 modelMat = glm::translate(glm::mat4(), glm::vec3((float)i.first.x * 16,
		                                                           (float)i.first.y * 16,
		                                                           (float)i.first.z * 16
		                                                           ));

		glUniformMatrix4fv(modelMatUni, 1, GL_FALSE, glm::value_ptr(modelMat));
		i.second->draw();
	}
}

void World::update() {
	//if the rebuild thread is not active and there are meshes to be updated
	if (!rebuildThreadActive && (mMeshUpdateQueueSoon.size_approx() > 0 ||
	                              mMeshUpdateQueue.size_approx() > 0)) {
		auto updatefunc = [&]() {
			glm::ivec3 pos;
			while (mMeshUpdateQueueSoon.try_dequeue(pos)) {
				ChunkMesh* mesh;
				mChunkMeshes.find(pos, mesh);
				if (mesh) {
					mesh->rebuildChunkGeometry();
				}
				getChunk(pos)->setQueuedForMeshRebuild(false);
			}
			
			while (mMeshUpdateQueue.try_dequeue(pos)) {
				ChunkMesh* mesh;
				mChunkMeshes.find(pos, mesh);
				if (mesh) {
					mesh->rebuildChunkGeometry();
				}
				getChunk(pos)->setQueuedForMeshRebuild(false);
			}
			rebuildThreadActive = false;
			/*
			while (!mChunkUpdateQueue.empty()) {
				auto& pos = mChunkUpdateQueue.back();
				ChunkMesh* mesh;
				mChunkMeshes.find(pos, mesh);
				mChunkUpdateQueue.pop_back();
				if (!mesh->rebuildChunkGeometry())
					queueChunkUpdate(pos);
			}
			rebuildThreadActive = false;
			*/
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
	/*
	int chunkCount = 1;
	chunkCount = 1;
	for (auto& i : mChunkMeshes) {
		std::cout << "\rUpdating chunk geometry, but better (" << chunkCount << "/" << mChunkMeshes.size() << ")" << std::flush;
		i.second.rebuildChunkGeometry();
		i.second.updateGeometry();
		++chunkCount;
	}
	*/
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

