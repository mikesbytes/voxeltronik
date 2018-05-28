/*
 * =====================================================================================
 *
 *       Filename:  terraingen.cpp
 *
 *    Description:  Terrain generator source
 *
 *        Version:  1.0
 *        Created:  05/08/2014 06:45:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "terraingen.h"
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

TerrainGen::TerrainGen() {
	std::shared_ptr<noise::NoiseModule> noise = std::make_shared<noise::Noise>(6969696);
	std::shared_ptr<noise::NoiseModule> yGrad = std::make_shared<noise::YGradient>(0.0, 128.0);
	mNoise = std::make_shared<noise::YTurbulence>(yGrad, noise, 15.0);

	mTerrainScale = 32.0;
}

void TerrainGen::generateChunk(Chunk* chunk) {
	auto chunkPos = chunk->getPos();
    //first pass
    for (int i = 0; i < 16; i++) { //x
        for (int j = 0; j < 16; j++) { //y
            for (int k = 0; k < 16; k++) { //z
				double nVal = mNoise->get3D(
						(double)(chunkPos.x * 16 + i),
						(double)(chunkPos.y * 16 + j),
						(double)(chunkPos.z * 16 + k));
				if (nVal <= 0.0) {
					if (mNoise->get3D((double)(chunkPos.x * 16 + i),
					                  (double)(chunkPos.y * 16 + j + 1),
					                  (double)(chunkPos.z * 16 + k)) >= 0.0)
						{
							chunk->setVoxelType(i, j, k, 2);
						} else {// block above is air
							chunk->setVoxelType(i,j,k,1);
					}
					chunk->getHeightMap()->pushUpdate(glm::ivec3(i,(chunkPos.y * 16) + j, k),false);
				} else {
					chunk->getHeightMap()->pushUpdate(glm::ivec3(i,(chunkPos.y * 16) + j, k), true);
					chunk->setVoxelType(i,j,k,0);
				}
            }
        }
    }
}

}
