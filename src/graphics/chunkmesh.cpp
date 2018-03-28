
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

	glBindBuffer(GL_ARRAY_BUFFER, mGeometryTexVBO);

	//vertices
	glVertexAttribIPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, 0);
	glEnableVertexAttribArray(0);


	//face attribs
	glBindBuffer(GL_ARRAY_BUFFER, mFaceAttribsVBO);

	glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, GL_FALSE, 0);
	glEnableVertexAttribArray(1);
}

void ChunkMesh::rebuildChunkGeometry() {
	// check if it's currently rebuilding
	if (mLocked) return;
	mLocked = true;

	//get the chunk from position
	auto chunk = mLinkedWorld.getChunk(mLinkedChunkPos); 
	
	// geometry format: x,y,z,u,v,i
	mGeometry.clear();
	mFaceAttribs.clear();
	mFaceCount = 0;
	int chunkSize = 16;
	
	for (int i = 0; i < chunkSize; ++i) {
		for (int j = 0; j < chunkSize; ++j) {
			for (int k = 0; k < chunkSize; ++k) {
				if (chunk->isVoxelSolid(i,j,k)) {

					//build face attrib
					// 0b00000000000000000xxxxxyyyyyzzzzz
					unsigned faceAttrib = 0;
					faceAttrib = faceAttrib | i;
					faceAttrib = (faceAttrib << 5) | j;
					faceAttrib = (faceAttrib << 5) | k;

					//lambda expression for adding vertices
					auto addFaceModel = [&](int faceIndex) {
						auto mesh = mVoxelModel.getFaceMesh(faceIndex);
						++mFaceCount;
						for (int l = 0; l < 6; ++l) {
							mGeometry.push_back(mesh[l]);
							mFaceAttribs.push_back(faceAttrib);
						}
					};

					if (!chunk->isVoxelSolid(i,j+1,k)) addFaceModel(0); // T
					if (!chunk->isVoxelSolid(i,j-1,k)) addFaceModel(1); // B
					if (!chunk->isVoxelSolid(i,j,k+1)) addFaceModel(2); // N
					if (!chunk->isVoxelSolid(i,j,k-1)) addFaceModel(3); // S
					if (!chunk->isVoxelSolid(i+1,j,k)) addFaceModel(4); // E
					if (!chunk->isVoxelSolid(i-1,j,k)) addFaceModel(5); // W
				}
			}
		}
	}
	mUpdated = true;
	mLocked = false;
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

		mGeometryFaceCount = mFaceCount;
		return true;
	}
	return false;
}

} //namespace vtk
