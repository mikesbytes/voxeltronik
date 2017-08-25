/*
 * =====================================================================================
 *
 *       Filename:  world.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/03/2014 09:53:00 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
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
}

bool World::isVoxelSolid(const int& x, const int& y, const int& z) {
    auto chunkPos = std::make_tuple(floor((float)x / (float)chunkSize), 
                                    floor((float)y / (float)chunkSize), 
                                    floor((float)z / (float)chunkSize));
    if (chunks.find(chunkPos) == chunks.end()) { //block is in nonexistant chunk
        return true;
    }

    int relPosX = x - std::get<0>(chunkPos) * chunkSize;
    int relPosY = y - std::get<1>(chunkPos) * chunkSize;
    int relPosZ = z - std::get<2>(chunkPos) * chunkSize;

    return chunks[chunkPos]->isVoxelSolid(relPosX, relPosY, relPosZ);
}

bool World::setVoxelType(const int& x, const int& y, const int& z, const unsigned& type, const bool& updateChunk) {
    auto chunkPos = std::make_tuple(floor((float)x / (float)chunkSize), floor((float)y / (float)chunkSize), floor((float)z / (float)chunkSize));
    if (chunks.find(chunkPos) == chunks.end()) { //block is in nonexistant chunk
        return false;
    }

    int relPosX = x - std::get<0>(chunkPos) * chunkSize;
    int relPosY = y - std::get<1>(chunkPos) * chunkSize;
    int relPosZ = z - std::get<2>(chunkPos) * chunkSize;

    chunks[chunkPos]->setVoxelType(relPosX, relPosY, relPosZ, type);
    if (updateChunk) queueChunkUpdate(chunkPos);
    return true;
}

unsigned World::getVoxelType(const glm::ivec3& pos) {
    auto chunkPos = std::make_tuple(floor((float)pos.x / (float)chunkSize), 
                                    floor((float)pos.y / (float)chunkSize), 
                                    floor((float)pos.z / (float)chunkSize));
    if (chunks.find(chunkPos) == chunks.end()) {
        return 0;
    }

    int relPosX = pos.x - std::get<0>(chunkPos) * chunkSize;
    int relPosY = pos.y - std::get<1>(chunkPos) * chunkSize;
    int relPosZ = pos.z - std::get<2>(chunkPos) * chunkSize;

    return chunks[chunkPos]->getVoxelType((unsigned)relPosX, (unsigned)relPosY, (unsigned)relPosZ);
}


bool World::makeChunk(const int& x, const int& y, const int& z) {
    auto pos = std::make_tuple(x, y, z);
    auto posvec = glm::ivec3(x, y, z);

    if (chunks.find(pos) != chunks.end()) { //chunk already exists
        return false;
    }

    auto newChunk = new Chunk(*this);
    chunks[pos] = newChunk;

    newChunk->setPos(posvec);

    //newChunk->renderer.linkedWorld = this;

    newChunk->renderer.init();
    newChunk->renderer.setChunkPosition(x,y,z);

    mChunkMeshes.emplace(posvec, ChunkMesh(*this, posvec));
 
    return true;
}

bool World::generateChunk(const int& x, const int& y, const int& z) {
    bool chunkMade = makeChunk(x,y,z);
    if (chunkMade) {
        terrain.generateChunk(chunks[std::make_tuple(x,y,z)]);
    }
    return chunkMade;
}


  Chunk* World::getChunk(const glm::ivec3& pos) {
    return chunks[std::make_tuple(pos.x, pos.y, pos.z)];
  }
  
void World::queueChunkUpdate(const int& x, const int& y, const int& z) {
    queueChunkUpdate(std::make_tuple(x,y,z));
}

void World::queueChunkUpdate(const iPos& pos) {
    if (chunks.find(pos) == chunks.end()) return; //chunk doesn't exist
    for (auto& i : chunkUpdateQueue) {
        if (i == pos) return; //chunk is already in queue, we don't need to update multiple times
    }
    chunkUpdateQueue.push_back(pos);
}

void World::draw() {
  /*
    for (auto& i : chunks) {
        glm::mat4 modelMat = glm::translate(glm::mat4(), glm::vec3(
                    (float)i.second->getPos().x * 16,
                    (float)i.second->getPos().y * 16,
                    (float)i.second->getPos().z * 16
                    ));
	
        glUniformMatrix4fv(modelMatUni, 1, GL_FALSE, glm::value_ptr(modelMat));
        i.second->renderer.drawChunk();
    }
  */
    for (auto& i : mChunkMeshes) {
        glm::mat4 modelMat = glm::translate(glm::mat4(), glm::vec3(
                    (float)i.first.x * 16,
                    (float)i.first.y * 16,
                    (float)i.first.z * 16
                    ));
	
        glUniformMatrix4fv(modelMatUni, 1, GL_FALSE, glm::value_ptr(modelMat));
        i.second.draw();
    }
}

void World::update() {
    while (!chunkUpdateQueue.empty()) {
        auto chunk = chunks[chunkUpdateQueue.back()];
        chunkUpdateQueue.pop_back(); 
        std::thread(&ChunkRenderer::updateGeometry, &chunk->renderer).detach();
    }

    for (auto& i : chunks) {
        i.second->renderer.updateVertexData();
    }
}

void World::forceGlobalGeometryUpdate() {
    int chunkCount = 1;
    /*
    for (auto& i : chunks) {
        std::cout << "\rUpdating chunk geometry (" << chunkCount << "/" << chunks.size() << ")" << std::flush;
        i.second->renderer.updateGeometry();
        chunkCount++;
        //i.second->renderer.updateVertexData();
    }
    */
    chunkCount = 1;
    for (auto& i : mChunkMeshes) {
      std::cout << "\rUpdating chunk geometry, but better (" << chunkCount << "/" << mChunkMeshes.size() << ")" << std::flush;
      i.second.rebuildChunkGeometry();
      ++chunkCount;
    }
}

}
