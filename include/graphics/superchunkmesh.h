// A chunk mesh for batching together other chunk meshes to reduce drawcalls
// chunks OR the supermesh but not both.
// To be used to group together chunks (i.e. 8^3 blocks of chunks)
// If rebuild is started when it's already rebuilding in another thread,it will
// restart rebuilding in the original thread.

#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace vtk {

class ChunkMesh;

class SuperChunkMesh {
public:
    SuperChunkMesh();
    bool addMesh(ChunkMesh* mesh);
    void rebuild();
protected:
    typedef struct {
	unsigned count;
	unsigned instanceCount;
	unsigned first;
	unsigned baseInstance;
    } DrawArraysIndirectCommand;
    
    std::vector<ChunkMesh*> mChunkMeshes;
    std::vector<glm::mat4> mModelMats;
};

}
