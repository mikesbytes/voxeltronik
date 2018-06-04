
#include "graphics/chunkmesh.h"

#include "world.h"
#include "chunk.h"

#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>


namespace vtk {

ChunkMesh::ChunkMesh(World& world, glm::ivec3 chunkPos) :
	mLinkedWorld(world),
	mLinkedChunkPos(chunkPos),
	mLocked(false)
{
	glGenBuffers(1, &mGeometryTexVBO);
	glGenBuffers(1, &mLightVBO);
	glGenBuffers(1, &mFaceAttribsVBO);

	glGenVertexArrays(1, &mVAO);

	glBindVertexArray(mVAO);

	//vertices
	glBindBuffer(GL_ARRAY_BUFFER, mGeometryTexVBO);
	glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, 0);
	glEnableVertexAttribArray(0);

	//face attribs
	glBindBuffer(GL_ARRAY_BUFFER, mFaceAttribsVBO);
	glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, GL_FALSE, 0);
	glEnableVertexAttribArray(1);

	//light
	glBindBuffer(GL_ARRAY_BUFFER, mLightVBO);
	glVertexAttribIPointer(2, 1, GL_UNSIGNED_SHORT, GL_FALSE, 0);
	glEnableVertexAttribArray(2);
}

bool ChunkMesh::rebuildChunkGeometry() {
	// check if it's currently rebuilding
	if (mLocked) return true;
	mLocked = true;

	//get the chunk from position
	auto chunk = mLinkedWorld.getChunk(mLinkedChunkPos);
	if (!chunk->isLoaded()) {
		mLocked = false; //unlock the builder before returning
		return false;
	}
	
	// geometry format: x,y,z,u,v,i
	mGeometry.clear();
	mLighting.clear();
	mFaceAttribs.clear();

	mFaceCount = 0;
	int chunkSize = 16;
	
	for (int i = 0; i < chunkSize; ++i) {
		for (int j = 0; j < chunkSize; ++j) {
			for (int k = 0; k < chunkSize; ++k) {
				if (chunk->isVoxelSolid(i,j,k)) {
					glm::ivec3 lpos(i,j,k);
					std::array<unsigned short, 27> surrounding_light;
					for (int l = 0; l < 27; ++l) {
						//magic to iterate through the 27 voxels around in proper order
						glm::ivec3 offset((l % 3) - 1, //x component
						                  (l / 9) - 1, //y component
						                  ((l % 9) / 3) - 1); //y component
						surrounding_light[l] = chunk->getLightPacked(glm::ivec3(i,j,k) + offset);
					}
					
					
					//build face attrib
					// 0b00000000000000000xxxxxyyyyyzzzzz
					unsigned faceAttrib = 0;
					faceAttrib = (faceAttrib << 5) | i;
					faceAttrib = (faceAttrib << 5) | j;
					faceAttrib = (faceAttrib << 5) | k;

					//lambda expression for adding vertices
					auto addFaceModel = [&](FaceDirection faceDirection) {
						auto mesh = mVoxelModel.getFaceMesh(static_cast<unsigned>(faceDirection));
						static std::vector<unsigned short> faceLighting;
						faceLighting.clear();
						mVoxelModel.getFaceLighting(faceLighting, faceDirection, surrounding_light, 0);
						unsigned texIndex = mLinkedWorld.voxelInfo.getTextureIndex(chunk->getVoxelType(i,j,k), faceDirection);
						unsigned faceAttribT = faceAttrib | (texIndex << 15); // pack texture index into faceAttrib
						unsigned lightVal = 0x000000FF;

						++mFaceCount;
						for (int l = 0; l < 6; ++l) {
							mGeometry.push_back(mesh[l]);
							mFaceAttribs.push_back(faceAttribT);
							mLighting.push_back(faceLighting[l]);
						}
					};


					if (!chunk->isVoxelSolid(i,j+1,k)) {
						addFaceModel(FaceDirection::TOP); // T
					}
					if (!chunk->isVoxelSolid(i,j-1,k)) {
						addFaceModel(FaceDirection::BOTTOM); // B
					}
					if (!chunk->isVoxelSolid(i,j,k+1)) {
						addFaceModel(FaceDirection::NORTH); // N
					}
					if (!chunk->isVoxelSolid(i,j,k-1)) {
						addFaceModel(FaceDirection::SOUTH); // S
					}
					if (!chunk->isVoxelSolid(i+1,j,k)) {
						addFaceModel(FaceDirection::EAST); // E
					}
					if (!chunk->isVoxelSolid(i-1,j,k)) {
						addFaceModel(FaceDirection::WEST); // W
					}
				}
			}
		}
	}
	mUpdated = true;
	mLocked = false;
	return true;
}

void ChunkMesh::rebuildChunkLighting() {
	//this doesn't do anything yet
}

void ChunkMesh::draw() {
	if (mFaceCount > 0) {
		updateGeometry();
		glBindVertexArray(mVAO);
		glDrawArrays(GL_TRIANGLES, 0, mGeometryFaceCount * 6);
	}
}

bool ChunkMesh::updateGeometry() {
	if (mUpdated) {
		mUpdated = false;

		//upload geometry, face attribs, and lighting
		glBindBuffer(GL_ARRAY_BUFFER, mGeometryTexVBO);
		glBufferData(GL_ARRAY_BUFFER,
		             sizeof(unsigned) * mGeometry.size(),
		             mGeometry.data(),
		             GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, mFaceAttribsVBO);
		glBufferData(GL_ARRAY_BUFFER,
		             sizeof(unsigned) * mFaceAttribs.size(),
		             mFaceAttribs.data(),
		             GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, mLightVBO);
		glBufferData(GL_ARRAY_BUFFER,
		             sizeof(unsigned) * mLighting.size(),
		             mLighting.data(),
		             GL_STATIC_DRAW);

		mGeometryFaceCount = mFaceCount;
		return true;
	}
	return false;
}

} //namespace vtk
