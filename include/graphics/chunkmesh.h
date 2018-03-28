#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <vector>

#include "graphics/voxelmodel.h"

namespace vtk {
class World;

class ChunkMesh {
public:
	ChunkMesh(World& world, glm::ivec3 linkedChunkPos);
	void rebuildChunkGeometry();
	void rebuildChunkLighting();
	void draw();
	bool updateGeometry(); //puts the geometry into the VBOs because OGL can't into threading

protected:
	//position of chunk since chunk does NOT have to exist to rebuild
	//if chunk is not loaded, rebuild will fail silently
	//this is useful for allowing you to unload distant chunks but still draw them
	//it's also faster to look up adjacent chunk positions when you have the position
	glm::ivec3 mLinkedChunkPos;
	World& mLinkedWorld;

	std::vector<float> geometry;
	std::vector<unsigned> mGeometry;
	std::vector<unsigned> mFaceAttribs;
	bool mLocked;
	bool mUpdated;
	unsigned mVAO;
	unsigned mGeometryTexVBO;
	unsigned mLightVBO;
	unsigned mFaceAttribsVBO;
	int mFaceCount;
	int mGeometryFaceCount;
	VoxelModel mVoxelModel;
};
  
}
