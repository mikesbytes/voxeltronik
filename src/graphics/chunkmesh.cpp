#include "graphics/chunkmesh.h"
#include "world.h"

namespace vtk {
  ChunkMesh::ChunkMesh(World& world, glm::ivec3 chunkPos) :
    mLinkedWorld(world),
    mLinkedChunkPos(chunkPos)
  {
  }

  void ChunkMesh::rebuildChunkGeometry(const unsigned& vao) {
    //get the chunk from position
    auto chunk = mLinkedWorld.getChunk(mLinkedChunkPos); 

    //temporary until I finish mesh providers
    float textUV [] = {
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
      0.0f, 0.0f, 1.0f, //bl
      1.0f, 0.0f, 1.0f //tr
    };
    float northModel [] = {
    //x     y     z     u     v
      0.0f, 1.0f, 1.0f, //tl
      1.0f, 1.0f, 1.0f, //tr
      0.0f, 1.0f, 1.0f, //bl
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
  }
  void ChunkMesh::rebuildChunkLighting(const unsigned& vao) {

  }
}
