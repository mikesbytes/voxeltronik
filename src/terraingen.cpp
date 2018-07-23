#include "terraingen.h"
#include "world.h"
#include "chunk.h"
#include "mathplus.h"
#include "terrain/noise.h"
#include "terrain/ygradient.h"
#include "terrain/yturbulence.h"
#include "heightmap.h"

#include <iostream>
#include <time.h>
#include <glm/glm.hpp>

namespace vtk {

TerrainGen::TerrainGen() :
	mDecorator(nullptr),
	mTerrainScale(32.0)
{
}

void TerrainGen::generateChunk(Chunk* chunk) {
	auto chunkPos = chunk->getPos();
    //first pass
    for (int i = 0; i < 16; i++) { //x
        for (int j = 0; j < 16; j++) { //y
            for (int k = 0; k < 16; k++) { //z
	            //mGenFunc();
	            /*
	            unsigned type = mGenFunc((double)(chunkPos.x * 16 + i),
	                                     (double)(chunkPos.y * 16 + j),
	                                     (double)(chunkPos.z * 16 + k));
	            */

	            unsigned type = 0;
	            if (mDecorator != nullptr) {
		            type = mDecorator->get3D(glm::ivec3(i,j,k) + chunkPos * 16);
	            }

				chunk->setVoxelType(i, j, k, type);
				if (!chunk->getWorld().voxelInfo.isTransparent(type)) {
					chunk->getHeightMap()->blockHeight(glm::ivec3(i,(chunkPos.y * 16) + j, k));
				}
					
				/* 
				double nVal = mNoise->get3D(
						(double)(chunkPos.x * 16 + i),
						(double)(chunkPos.y * 16 + j),
						(double)(chunkPos.z * 16 + k));
				if (nVal <= 0.0) {
					if (mNoise->get3D((double)(chunkPos.x * 16 + i),
					                  (double)(chunkPos.y * 16 + j + 1),
					                  (double)(chunkPos.z * 16 + k)) >= 0.0)
						{
							chunk->setVoxelType(i, j, k, 3);
						} else {// block above is air
							chunk->setVoxelType(i,j,k,1);
					}
					chunk->getHeightMap()->blockHeight(glm::ivec3(i,(chunkPos.y * 16) + j, k));
				} 
				*/
            }
        }
    }
}

void TerrainGen::setDecorator(noise::Decorator *dec) {
	mDecorator = dec;
}


void TerrainGen::registerScriptInterface(sol::state &lua) {
	lua.new_usertype<TerrainGen>("TerrainGen",
	                             "set_decorator", &TerrainGen::setDecorator);
}

}
