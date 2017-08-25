#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace vtk {
  class World;

  class ChunkMesh {
  public:
    ChunkMesh(World& world, glm::ivec3 linkedChunkPos);
    void rebuildChunkGeometry(const unsigned& vao);
    void rebuildChunkLighting(const unsigned& vao);
    void draw();

  protected:
    //position of chunk since chunk does NOT have to exist to rebuild
    //if chunk is not loaded, rebuild will fail silently
    //this is useful for allowing you to unload distant chunks but still draw them
    //it's also faster to look up adjacent chunk positions when you have the position
    glm::ivec3 mLinkedChunkPos;
    World& mLinkedWorld;

    unsigned mVAO;
    unsigned mGeometryTexVBO;
    unsigned mLightVBO;
    int mFaceCount;
  };
  
}
