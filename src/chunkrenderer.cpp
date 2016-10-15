/*
 * =====================================================================================
 *
 *       Filename:  chunkrenderer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/04/2014 11:34:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "chunkrenderer.h"

#include "chunk.h"
#include "world.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>

namespace vtk {

void ChunkRenderer::init() {
    hasGeometryChanged = false;
    hasTexturesChanged = false;
    hasLightingChanged = false;
    updatingGeometry = false;

    chunkSize = 16;
    lightingMethod = 0;
    voxelSize = 1.0f;

    //generate VBOs
    for (auto& i : vertexBuffers) {
        for(auto& j : i) {
            glGenBuffers(1, &j);
        }
    }

    //generate VAOs
    for (auto& i : vertexArrays) {
        glGenVertexArrays(1, &i);
    }
   

    //bind VAOs
    for (int i = 0; i < 6; i++) {
        glBindVertexArray(vertexArrays[i]);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i][0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);
        
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i][1]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i][2]);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(2);
    }
}

void ChunkRenderer::drawChunk() {
    for (int i = 0; i < 6; i++) {
        glBindVertexArray(vertexArrays[i]);
        glDrawArrays(GL_TRIANGLES, 0, vertexCounts[i]);
    }
}

void ChunkRenderer::setChunkPosition(const int&x, const int& y, const int&z) {
    chunkPos = std::make_tuple(x, y, z);
}

void ChunkRenderer::updateGeometry() {
    updatingGeometry = true;

    for (auto& i : geometryVertices)
        i.clear();

    for (auto& i : faceCounts)
        i = 0;

    for (unsigned i = 0; i < chunkSize; i++) {
        for (unsigned j = 0; j < chunkSize; j++) {
            for (unsigned k = 0; k < chunkSize; k++) {
                if (linkedChunk->isVoxelSolid(i,j,k)) {
                    if (!linkedChunk->isVoxelSolid(i+1,j,k)) {
                        //right face
                        //top left
                        addVertex(geometryVertices[0], i * voxelSize + voxelSize, j * voxelSize + voxelSize, k * voxelSize + voxelSize);
                        //top right
                        addVertex(geometryVertices[0], i * voxelSize + voxelSize, j * voxelSize + voxelSize, k * voxelSize);
                        //bottom left
                        addVertex(geometryVertices[0], i * voxelSize + voxelSize, j * voxelSize,             k * voxelSize + voxelSize);
                        //bottom right
                        addVertex(geometryVertices[0], i * voxelSize + voxelSize, j * voxelSize,             k * voxelSize);
                        //bottom left
                        addVertex(geometryVertices[0], i * voxelSize + voxelSize, j * voxelSize,             k * voxelSize + voxelSize);
                        //top right
                        addVertex(geometryVertices[0], i * voxelSize + voxelSize, j * voxelSize + voxelSize, k * voxelSize);
                        faceCounts[0]++;
                    }
                    if (!linkedChunk->isVoxelSolid(i-1,j,k)) {
                        //left face
                        //top left
                        addVertex(geometryVertices[1], i * voxelSize,             j * voxelSize + voxelSize, k * voxelSize);
                        //top right
                        addVertex(geometryVertices[1], i * voxelSize,             j * voxelSize + voxelSize, k * voxelSize + voxelSize);
                        //bottom left
                        addVertex(geometryVertices[1], i * voxelSize,             j * voxelSize,             k * voxelSize);
                        //bottom right
                        addVertex(geometryVertices[1], i * voxelSize,             j * voxelSize,             k * voxelSize + voxelSize);
                        //bottom left
                        addVertex(geometryVertices[1], i * voxelSize,             j * voxelSize,             k * voxelSize);
                        //top right
                        addVertex(geometryVertices[1], i * voxelSize,             j * voxelSize + voxelSize, k * voxelSize + voxelSize);
                        faceCounts[1]++;
                    }

                    if (!linkedChunk->isVoxelSolid(i,j+1,k)) {
                        //top face
                        //bottom left
                        addVertex(geometryVertices[2], i * voxelSize,             j * voxelSize + voxelSize, k * voxelSize + voxelSize);
                        //top left
                        addVertex(geometryVertices[2], i * voxelSize,             j * voxelSize + voxelSize, k * voxelSize);
                        //bottom right
                        addVertex(geometryVertices[2], i * voxelSize + voxelSize, j * voxelSize + voxelSize, k * voxelSize + voxelSize);
                        //top right
                        addVertex(geometryVertices[2], i * voxelSize + voxelSize, j * voxelSize + voxelSize, k * voxelSize);
                        //bottom right
                        addVertex(geometryVertices[2], i * voxelSize + voxelSize, j * voxelSize + voxelSize, k * voxelSize + voxelSize);
                        //top left
                        addVertex(geometryVertices[2], i * voxelSize,             j * voxelSize + voxelSize, k * voxelSize);
                        faceCounts[2]++;
                    }
                    
                    if (!linkedChunk->isVoxelSolid(i,j-1,k)) {
                        //bottom face
                        //top right
                        addVertex(geometryVertices[3], i * voxelSize + voxelSize, j * voxelSize,             k * voxelSize + voxelSize);
                        //bottom right
                        addVertex(geometryVertices[3], i * voxelSize + voxelSize, j * voxelSize,             k * voxelSize);
                        //top left
                        addVertex(geometryVertices[3], i * voxelSize,             j * voxelSize,             k * voxelSize + voxelSize);
                        //bottom left
                        addVertex(geometryVertices[3], i * voxelSize,             j * voxelSize,             k * voxelSize);
                        //top left
                        addVertex(geometryVertices[3], i * voxelSize,             j * voxelSize,             k * voxelSize + voxelSize);
                        //bottom right
                        addVertex(geometryVertices[3], i * voxelSize + voxelSize, j * voxelSize,             k * voxelSize);
                        faceCounts[3]++;
                    }

                    if (!linkedChunk->isVoxelSolid(i,j,k-1)) {
                        //back face
                        //top left
                        addVertex(geometryVertices[4], i * voxelSize + voxelSize, j * voxelSize + voxelSize, k * voxelSize);
                        //top right
                        addVertex(geometryVertices[4], i * voxelSize,             j * voxelSize + voxelSize, k * voxelSize);
                        //bottom left
                        addVertex(geometryVertices[4], i * voxelSize + voxelSize, j * voxelSize,             k * voxelSize);
                        //bottom right
                        addVertex(geometryVertices[4], i * voxelSize,             j * voxelSize,             k * voxelSize);
                        //bottom left
                        addVertex(geometryVertices[4], i * voxelSize + voxelSize, j * voxelSize,             k * voxelSize);
                        //top right
                        addVertex(geometryVertices[4], i * voxelSize,             j * voxelSize + voxelSize, k * voxelSize);
                        faceCounts[4]++;
                    }

                    if (!linkedChunk->isVoxelSolid(i,j,k+1)) {
                        //front face
                        //bottom right
                        addVertex(geometryVertices[5], i * voxelSize + voxelSize, j * voxelSize,             k * voxelSize + voxelSize);
                        //bottom left
                        addVertex(geometryVertices[5], i * voxelSize,             j * voxelSize,             k * voxelSize + voxelSize);
                        //top right
                        addVertex(geometryVertices[5], i * voxelSize + voxelSize, j * voxelSize + voxelSize, k * voxelSize + voxelSize);
                        //top left
                        addVertex(geometryVertices[5], i * voxelSize,             j * voxelSize + voxelSize, k * voxelSize + voxelSize);
                        //top right
                        addVertex(geometryVertices[5], i * voxelSize + voxelSize, j * voxelSize + voxelSize, k * voxelSize + voxelSize);
                        //bottom left
                        addVertex(geometryVertices[5], i * voxelSize,             j * voxelSize,             k * voxelSize + voxelSize);
                        faceCounts[5]++;
                    }
                }
            }
        }
    }

    hasGeometryChanged = true;

    updateTexCoords();
    updateLighting();

    
    updatingGeometry = false;
}

void ChunkRenderer::updateTexCoords() {
	auto linkedWorld = linkedChunk->getWorld();
    for (auto& i : textureCoords)
        i.clear();

    for (unsigned i = 0; i < chunkSize; i++) {
        for (unsigned j = 0; j < chunkSize; j++) {
            for (unsigned k = 0; k < chunkSize; k++) {
                if (linkedChunk->isVoxelSolid(i,j,k)) {
                if (!linkedChunk->isVoxelSolid(i+1,j,k)) {
                    //right face
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[0], linkedChunk->getWorldCoords(i,j,k), Face3D::RIGHT, Corner2D::TOPLEFT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[0], linkedChunk->getWorldCoords(i,j,k), Face3D::RIGHT, Corner2D::TOPRIGHT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[0], linkedChunk->getWorldCoords(i,j,k), Face3D::RIGHT, Corner2D::BOTTOMLEFT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[0], linkedChunk->getWorldCoords(i,j,k), Face3D::RIGHT, Corner2D::BOTTOMRIGHT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[0], linkedChunk->getWorldCoords(i,j,k), Face3D::RIGHT, Corner2D::BOTTOMLEFT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[0], linkedChunk->getWorldCoords(i,j,k), Face3D::RIGHT, Corner2D::TOPRIGHT);
                }
                if (!linkedChunk->isVoxelSolid(i-1,j,k)) {
                    //left face
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[1], linkedChunk->getWorldCoords(i,j,k), Face3D::LEFT, Corner2D::TOPLEFT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[1], linkedChunk->getWorldCoords(i,j,k), Face3D::LEFT, Corner2D::TOPRIGHT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[1], linkedChunk->getWorldCoords(i,j,k), Face3D::LEFT, Corner2D::BOTTOMLEFT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[1], linkedChunk->getWorldCoords(i,j,k), Face3D::LEFT, Corner2D::BOTTOMRIGHT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[1], linkedChunk->getWorldCoords(i,j,k), Face3D::LEFT, Corner2D::BOTTOMLEFT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[1], linkedChunk->getWorldCoords(i,j,k), Face3D::LEFT, Corner2D::TOPRIGHT);
                }
                if (!linkedChunk->isVoxelSolid(i,j+1,k)) {
                //top face
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[2], linkedChunk->getWorldCoords(i,j,k), Face3D::TOP, Corner2D::BOTTOMLEFT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[2], linkedChunk->getWorldCoords(i,j,k), Face3D::TOP, Corner2D::TOPLEFT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[2], linkedChunk->getWorldCoords(i,j,k), Face3D::TOP, Corner2D::BOTTOMRIGHT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[2], linkedChunk->getWorldCoords(i,j,k), Face3D::TOP, Corner2D::TOPRIGHT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[2], linkedChunk->getWorldCoords(i,j,k), Face3D::TOP, Corner2D::BOTTOMRIGHT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[2], linkedChunk->getWorldCoords(i,j,k), Face3D::TOP, Corner2D::TOPLEFT);
                }
                if (!linkedChunk->isVoxelSolid(i,j-1,k)) {
                //bottom face
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[3], linkedChunk->getWorldCoords(i,j,k), Face3D::BOTTOM, Corner2D::TOPRIGHT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[3], linkedChunk->getWorldCoords(i,j,k), Face3D::BOTTOM, Corner2D::BOTTOMRIGHT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[3], linkedChunk->getWorldCoords(i,j,k), Face3D::BOTTOM, Corner2D::TOPLEFT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[3], linkedChunk->getWorldCoords(i,j,k), Face3D::BOTTOM, Corner2D::BOTTOMLEFT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[3], linkedChunk->getWorldCoords(i,j,k), Face3D::BOTTOM, Corner2D::TOPLEFT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[3], linkedChunk->getWorldCoords(i,j,k), Face3D::BOTTOM, Corner2D::BOTTOMRIGHT);
                }
                if (!linkedChunk->isVoxelSolid(i,j,k-1)) {
                //back face
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[4], linkedChunk->getWorldCoords(i,j,k), Face3D::BACK, Corner2D::TOPLEFT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[4], linkedChunk->getWorldCoords(i,j,k), Face3D::BACK, Corner2D::TOPRIGHT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[4], linkedChunk->getWorldCoords(i,j,k), Face3D::BACK, Corner2D::BOTTOMLEFT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[4], linkedChunk->getWorldCoords(i,j,k), Face3D::BACK, Corner2D::BOTTOMRIGHT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[4], linkedChunk->getWorldCoords(i,j,k), Face3D::BACK, Corner2D::BOTTOMLEFT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[4], linkedChunk->getWorldCoords(i,j,k), Face3D::BACK, Corner2D::TOPRIGHT);

                }
                if (!linkedChunk->isVoxelSolid(i,j,k+1)) {
                //front face
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[5], linkedChunk->getWorldCoords(i,j,k), Face3D::FRONT, Corner2D::BOTTOMRIGHT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[5], linkedChunk->getWorldCoords(i,j,k), Face3D::FRONT, Corner2D::BOTTOMLEFT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[5], linkedChunk->getWorldCoords(i,j,k), Face3D::FRONT, Corner2D::TOPRIGHT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[5], linkedChunk->getWorldCoords(i,j,k), Face3D::FRONT, Corner2D::TOPLEFT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[5], linkedChunk->getWorldCoords(i,j,k), Face3D::FRONT, Corner2D::TOPRIGHT);
                    linkedWorld.voxelInfo.pushTexCoordFromWorldCoords(textureCoords[5], linkedChunk->getWorldCoords(i,j,k), Face3D::FRONT, Corner2D::BOTTOMLEFT);
                }
                }
            }
        }
    }
    hasTexturesChanged = true;
}

void ChunkRenderer::updateLighting() {
    for (auto& i : lightingData)
        i.clear();

    for (unsigned i = 0; i < chunkSize; i++) {
        for (unsigned j = 0; j < chunkSize; j++) {
            for (unsigned k = 0; k < chunkSize; k++) {
                if (linkedChunk->isVoxelSolid(i,j,k)) {
                    if (lightingMethod == 0) { //fullbright lighting
                        //face                       U  ,  V  ,  I
                        //right face                
                        if (!linkedChunk->isVoxelSolid(i+1,j,k)) {
                        addVertex(lightingData[0], 0.8f, 0.8f, 0.8f); //TL
                        addVertex(lightingData[0], 0.8f, 0.8f, 0.8f); //TR
                        addVertex(lightingData[0], 0.8f, 0.8f, 0.8f); //BL
                        addVertex(lightingData[0], 0.8f, 0.8f, 0.8f); //BR
                        addVertex(lightingData[0], 0.8f, 0.8f, 0.8f); //BL
                        addVertex(lightingData[0], 0.8f, 0.8f, 0.8f); //TR
                        }
                        if (!linkedChunk->isVoxelSolid(i-1,j,k)) {
                        //left face
                        addVertex(lightingData[1], 0.8f, 0.8f, 0.8f); //TL
                        addVertex(lightingData[1], 0.8f, 0.8f, 0.8f); //TR
                        addVertex(lightingData[1], 0.8f, 0.8f, 0.8f); //BL
                        addVertex(lightingData[1], 0.8f, 0.8f, 0.8f); //BR
                        addVertex(lightingData[1], 0.8f, 0.8f, 0.8f); //BL
                        addVertex(lightingData[1], 0.8f, 0.8f, 0.8f); //TR
                        }
                        if (!linkedChunk->isVoxelSolid(i,j+1,k)) {
                        //top face
                        addVertex(lightingData[2], 1.0f, 1.0f, 1.0f); //TL
                        addVertex(lightingData[2], 1.0f, 1.0f, 1.0f); //TR
                        addVertex(lightingData[2], 1.0f, 1.0f, 1.0f); //BL
                        addVertex(lightingData[2], 1.0f, 1.0f, 1.0f); //BR
                        addVertex(lightingData[2], 1.0f, 1.0f, 1.0f); //BL
                        addVertex(lightingData[2], 1.0f, 1.0f, 1.0f); //TR
                        }
                        if (!linkedChunk->isVoxelSolid(i,j-1,k)) {
                        //bottom face
                        addVertex(lightingData[3], 0.5f, 0.5f, 0.5f); //TL
                        addVertex(lightingData[3], 0.5f, 0.5f, 0.5f); //TR
                        addVertex(lightingData[3], 0.5f, 0.5f, 0.5f); //BL
                        addVertex(lightingData[3], 0.5f, 0.5f, 0.5f); //BR
                        addVertex(lightingData[3], 0.5f, 0.5f, 0.5f); //BL
                        addVertex(lightingData[3], 0.5f, 0.5f, 0.5f); //TR
                        }
                        if (!linkedChunk->isVoxelSolid(i,j,k-1)) {
                        //back face
                        addVertex(lightingData[4], 0.7f, 0.7f, 0.7f); //TL
                        addVertex(lightingData[4], 0.7f, 0.7f, 0.7f); //TR
                        addVertex(lightingData[4], 0.7f, 0.7f, 0.7f); //BL
                        addVertex(lightingData[4], 0.7f, 0.7f, 0.7f); //BR
                        addVertex(lightingData[4], 0.7f, 0.7f, 0.7f); //BL
                        addVertex(lightingData[4], 0.7f, 0.7f, 0.7f); //TR
                        }
                        if (!linkedChunk->isVoxelSolid(i,j,k+1)) {
                        //front face
                        addVertex(lightingData[5], 0.9f, 0.9f, 0.9f); //TL
                        addVertex(lightingData[5], 0.9f, 0.9f, 0.9f); //TR
                        addVertex(lightingData[5], 0.9f, 0.9f, 0.9f); //BL
                        addVertex(lightingData[5], 0.9f, 0.9f, 0.9f); //BR
                        addVertex(lightingData[5], 0.9f, 0.9f, 0.9f); //BL
                        addVertex(lightingData[5], 0.9f, 0.9f, 0.9f); //TR
                        }
                    } else if (lightingMethod == 1) { //blocky lighting (Need to implement!)

                    } else if (lightingMethod == 2) { //smooth lighting (Need to implement!)

                    }
                }
            }
        }
    }
    
    hasLightingChanged = true;
}

void ChunkRenderer::updateVertexData() { 
    if (!updatingGeometry) { 
        //std::cout << "testing";
        for (unsigned i = 0; i < 6; i++) {
            if (hasGeometryChanged) {
                glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i][0]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * geometryVertices[i].size(), geometryVertices[i].data(), GL_STATIC_DRAW);
                //hasGeometryChanged = false;
            }
            if (hasTexturesChanged) {
                glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i][1]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * textureCoords[i].size(), textureCoords[i].data(), GL_STATIC_DRAW);
                //hasTexturesChanged = false;
            }
            if (hasLightingChanged) {
                glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i][2]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * lightingData[i].size(), lightingData[i].data(), GL_STATIC_DRAW);
                //hasLightingChanged = false;
            }
        }

        if (hasGeometryChanged) hasGeometryChanged = false;
        if (hasTexturesChanged) hasTexturesChanged = false;
        if (hasLightingChanged) hasLightingChanged = false;

        for (int i = 0; i < 6; i++) {
            vertexCounts[i] = faceCounts[i] * 6;
        }
    }
}

void ChunkRenderer::addVertex(std::vector<float>& data, const float& x, const float& y, const float& z) {
    data.push_back(x);
    data.push_back(y);
    data.push_back(z);
}

}
