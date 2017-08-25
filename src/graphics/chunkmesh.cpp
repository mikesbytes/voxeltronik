
#include "graphics/chunkmesh.h"

#include "world.h"
#include "chunk.h"

#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>


namespace vtk {
  ChunkMesh::ChunkMesh(World& world, glm::ivec3 chunkPos) :
    mLinkedWorld(world),
    mLinkedChunkPos(chunkPos)
  {
    glGenBuffers(1, &mGeometryTexVBO);
    glGenBuffers(1, &mLightVBO);

    glGenVertexArrays(1, &mVAO);

    glBindVertexArray(mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mGeometryTexVBO);


    //vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    //tex uv coords
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
  }

  void ChunkMesh::rebuildChunkGeometry() {
    //get the chunk from position
    auto chunk = mLinkedWorld.getChunk(mLinkedChunkPos); 

    //temporary until I finish mesh providers
    float texUV [] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	1.0f, 1.0f
    };
    float topModel [] = {
    //x     y     z    
      0.0f, 1.0f, 0.0f, //tl
      1.0f, 1.0f, 0.0f, //tr
      0.0f, 1.0f, 1.0f, //bl
      1.0f, 1.0f, 1.0f, //br
      0.0f, 1.0f, 1.0f, //bl
      1.0f, 1.0f, 0.0f //tr
    };
    float bottomModel [] = {
    //x     y     z     u     v
      0.0f, 0.0f, 1.0f, //tl
      1.0f, 0.0f, 1.0f, //tr
      0.0f, 0.0f, 0.0f, //bl
      1.0f, 0.0f, 0.0f, //br
      0.0f, 0.0f, 0.0f, //bl
      1.0f, 0.0f, 1.0f //tr
    };
    float northModel [] = {
    //x     y     z     u     v
      0.0f, 1.0f, 1.0f, //tl
      1.0f, 1.0f, 1.0f, //tr
      0.0f, 0.0f, 1.0f, //bl
      1.0f, 0.0f, 1.0f, //br
      0.0f, 0.0f, 1.0f, //bl
      1.0f, 1.0f, 1.0f //tr
    };
    float southModel [] = {
    //x     y     z     u     v
      1.0f, 1.0f, 0.0f, //tl
      0.0f, 1.0f, 0.0f, //tr
      1.0f, 0.0f, 0.0f, //bl
      0.0f, 0.0f, 0.0f, //br
      1.0f, 0.0f, 0.0f, //bl
      0.0f, 1.0f, 0.0f //tr
    };
    float eastModel [] = {
    //x     y     z     u     v
      1.0f, 1.0f, 1.0f, //tl
      1.0f, 1.0f, 0.0f, //tr
      1.0f, 0.0f, 1.0f, //bl
      1.0f, 0.0f, 0.0f, //br
      1.0f, 0.0f, 1.0f, //bl
      1.0f, 1.0f, 0.0f //tr
    };
    float westModel [] = {
    //x     y     z     u     v
      0.0f, 1.0f, 0.0f, //tl
      0.0f, 1.0f, 1.0f, //tr
      0.0f, 0.0f, 0.0f, //bl
      0.0f, 0.0f, 1.0f, //br
      0.0f, 0.0f, 0.0f, //bl
      0.0f, 1.0f, 1.0f //tr
    };

    // geometry format: x,y,z,u,v,i
    std::vector<float> geometry;
    mFaceCount = 0;
    int chunkSize = 16; // TODO: make this not hardcoded

    for (int i = 0; i < chunkSize; ++i) {
      for (int j = 0; j < chunkSize; ++j) {
	for (int k = 0; k < chunkSize; ++k) {
	  //for each voxel in the chunk...
	  // TODO: move this code into a mesh provider system

	  //top
	  if (chunk->isVoxelSolid(i,j,k)) { //check that it's solid
	    if (!chunk->isVoxelSolid(i, j + 1, k)) {
	      ++mFaceCount;
	      for (int l = 0; l < 6; ++l) {
		geometry.push_back(topModel[(l * 3)] + (float)i);
		geometry.push_back(topModel[(l * 3) + 1] + (float)j);
		geometry.push_back(topModel[(l * 3) + 2] + (float)k);
		geometry.push_back(texUV[l*2]);
		geometry.push_back(texUV[(l*2)+1]);
		geometry.push_back(mLinkedWorld.voxelInfo.getTexIndexFromID(2, Face3D::TOP));
	      }
	    } 
	  }

	  //bottom
	  if (chunk->isVoxelSolid(i,j,k)) { //check that it's solid
	    if (!chunk->isVoxelSolid(i, j - 1, k)) {
	      ++mFaceCount;
	      for (int l = 0; l < 6; ++l) {
		geometry.push_back(bottomModel[(l * 3)] + (float)i);
		geometry.push_back(bottomModel[(l * 3) + 1] + (float)j);
		geometry.push_back(bottomModel[(l * 3) + 2] + (float)k);
		geometry.push_back(texUV[l*2]);
		geometry.push_back(texUV[(l*2)+1]);
		geometry.push_back(mLinkedWorld.voxelInfo.getTexIndexFromID(2, Face3D::TOP));
	      }
	    } 
	  }

	  //north
	  if (chunk->isVoxelSolid(i,j,k)) { //check that it's solid
	    if (!chunk->isVoxelSolid(i, j, k + 1)) {
	      ++mFaceCount;
	      for (int l = 0; l < 6; ++l) {
		geometry.push_back(northModel[(l * 3)] + (float)i);
		geometry.push_back(northModel[(l * 3) + 1] + (float)j);
		geometry.push_back(northModel[(l * 3) + 2] + (float)k);
		geometry.push_back(texUV[l*2]);
		geometry.push_back(texUV[(l*2)+1]);
		geometry.push_back(mLinkedWorld.voxelInfo.getTexIndexFromID(2, Face3D::TOP));
	      }
	    } 
	  }
	  //south
	  if (chunk->isVoxelSolid(i,j,k)) { //check that it's solid
	    if (!chunk->isVoxelSolid(i, j, k - 1)) {
	      ++mFaceCount;
	      for (int l = 0; l < 6; ++l) {
		geometry.push_back(southModel[(l * 3)] + (float)i);
		geometry.push_back(southModel[(l * 3) + 1] + (float)j);
		geometry.push_back(southModel[(l * 3) + 2] + (float)k);
		geometry.push_back(texUV[l*2]);
		geometry.push_back(texUV[(l*2)+1]);
		geometry.push_back(mLinkedWorld.voxelInfo.getTexIndexFromID(2, Face3D::TOP));
	      }
	    } 
	  }
	    //east
	  if (chunk->isVoxelSolid(i,j,k)) { //check that it's solid
	    if (!chunk->isVoxelSolid(i + 1, j, k)) {
	      ++mFaceCount;
	      for (int l = 0; l < 6; ++l) {
		geometry.push_back(eastModel[(l * 3)] + (float)i);
		geometry.push_back(eastModel[(l * 3) + 1] + (float)j);
		geometry.push_back(eastModel[(l * 3) + 2] + (float)k);
		geometry.push_back(texUV[l*2]);
		geometry.push_back(texUV[(l*2)+1]);
		geometry.push_back(mLinkedWorld.voxelInfo.getTexIndexFromID(2, Face3D::TOP));
	      }
	    } 
	  }

	    //west
	  if (chunk->isVoxelSolid(i,j,k)) { //check that it's solid
	    if (!chunk->isVoxelSolid(i - 1, j, k)) {
	      ++mFaceCount;
	      for (int l = 0; l < 6; ++l) {
		geometry.push_back(westModel[(l * 3)] + (float)i);
		geometry.push_back(westModel[(l * 3) + 1] + (float)j);
		geometry.push_back(westModel[(l * 3) + 2] + (float)k);
		geometry.push_back(texUV[l*2]);
		geometry.push_back(texUV[(l*2)+1]);
		geometry.push_back(mLinkedWorld.voxelInfo.getTexIndexFromID(2, Face3D::TOP));
	      }
	    } 
	  }
	}
      }
    }

    glBindBuffer(GL_ARRAY_BUFFER, mGeometryTexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * geometry.size(), geometry.data(), GL_STATIC_DRAW);
    
  }

  void ChunkMesh::rebuildChunkLighting() {
    //this doesn't do anything yet
  }

  void ChunkMesh::draw() {
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, mFaceCount * 6);
  }
}
