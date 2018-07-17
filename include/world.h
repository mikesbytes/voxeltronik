#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <unordered_map>
#include <vector>
#include <deque>
#include <glm/glm.hpp>

#include "voxelutils.h"
#include "voxelinfo.h"
#include "voxelmath.h"
#include "terraingen.h"
#include "heightmap.h"
#include "graphics/chunkmesh.h"
#include "graphics/drawable.h"
#include "graphics/rendertask.h"

#include "cuckoohash_map.hh"
#include "concurrentqueue.h"

//forward declarations
namespace sol {
class state;
}

namespace vtk {

class Chunk;

class World : public Drawable {
	friend class Chunk;
public:
    World();

	//break and place voxels, handles all chunk updating automatically
	unsigned breakVoxel(const glm::ivec3& pos);
	bool placeVoxel(const glm::ivec3& pos, const unsigned& id);
	
    bool isVoxelSolid(const int& x, const int& y, const int& z);

	//sets voxel type. does NOT automatically update ANYTHING, mostly for internal use or writing
	//youjr own break and place funcs
    bool setVoxelType(const int& x, const int& y, const int& z, const unsigned& type, const bool& updateChunk = false);

    unsigned getVoxelType(const glm::ivec3& pos);

	Chunk* makeChunk(const int& x, const int& y, const int& z, bool insertAfter = true);
    bool generateChunk(const int& x, const int& y, const int& z);
	bool insertChunk(Chunk* chunk);

    Chunk* getChunk(const glm::ivec3& pos);

	HeightMap* getHeightMap(const glm::ivec2& pos);

	//get max height of world at pos(x,z)
	int getHeight(const glm::ivec2& pos);

    void queueChunkUpdate(const int& x, const int& y, const int& z, const bool& highpriority = false);
    void queueChunkUpdate(const glm::ivec3& pos, const bool& highpriority = false);

	void queueChunkLoad(const glm::ivec3& pos);

    void draw(RenderTask& task);
    void update();


	void queueChunkLoadsAroundPoint(const glm::vec3& point, const int& chunkRadius);

	static void registerScriptInterface(::sol::state& lua);

	//std::unordered_map<glm::ivec3, Chunk*, ivec3Hash> mChunks;
	cuckoohash_map<glm::ivec3, Chunk*, ivec3Hash> mChunks; //chunks
	cuckoohash_map<glm::ivec3, ChunkMesh*, ivec3Hash> mChunkMeshes; //meshes
	cuckoohash_map<glm::ivec2, HeightMap*, ivec2Hash> mHeightMaps; //heightmaps
	
	std::deque<glm::ivec3> mChunkLoadQueue;

	// mesh update queues
	moodycamel::ConcurrentQueue<glm::ivec3> mMeshUpdateQueue; // regular one
	moodycamel::ConcurrentQueue<glm::ivec3> mMeshUpdateQueueSoon; // high priority one

	//heightmap update queue
	moodycamel::ConcurrentQueue<glm::ivec2> mHeightMapUpdateQueue;

    unsigned chunkSize;
    float voxelSize;

    int modelMatUni;

    TerrainGen terrain;
    VoxelInfo voxelInfo;
    VoxelMath voxelMath;
    bool rebuildThreadActive;
	bool mLoadThreadActive;

protected:
	GLuint mShader;
	GLuint mViewMat;
	GLuint mProjMat;
	GLuint mModelMat;
};

}
