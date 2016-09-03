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

#include <iostream>
#include <time.h>
#include <glm/glm.hpp>

namespace vtk {

TerrainGen::TerrainGen() {
    const1Mod.SetConstValue(1.0);
    const0Mod.SetConstValue(0.0);
    constNeg1Mod.SetConstValue(-1.0);

    gradientMod.setStop(0, 0.0);
    gradientMod.setStop(1, 1.0);

    terrainTurb.SetSourceModule(0, gradientMod);
    terrainTurb.power = .3;
    terrainTurb.yNoise.SetOctaveCount(3);
    terrainTurb.yNoise.SetFrequency(1.5);
    terrainTurb.yNoise.SetSeed(time(0));

    selectorMod.SetControlModule(terrainTurb);
    selectorMod.SetSourceModule(0, constNeg1Mod);
    selectorMod.SetSourceModule(1, const1Mod);
    selectorMod.SetBounds(0.0, 1000.0);

    terrainScale = 1.0 / 128.0;
}

void TerrainGen::generateChunk(Chunk* chunk) {
    //first pass
    for (int i = 0; i < 16; i++) { //x
        for (int j = 0; j < 16; j++) { //y
            for (int k = 0; k < 16; k++) { //z
                glm::dvec3 pos((double)(chunk->chunkPos.x * 16 + i) * terrainScale,
                               (double)(chunk->chunkPos.y * 16 + j) * terrainScale,
                               (double)(chunk->chunkPos.z * 16 + k) * terrainScale);
                if (selectorMod.GetValue(pos.x, pos.y, pos.z) <= 0.0) {
                    if (selectorMod.GetValue(pos.x, (double)(chunk->chunkPos.y * 16 + j + 1) * terrainScale, pos.z) >= 0.0) {
                        chunk->setVoxelType(i,j,k,2);
                    } else {
                        chunk->setVoxelType(i,j,k,1);
                    }
                }
            }
        }
    }

    //grass on top
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 16; k++) {
                if (chunk->getVoxelType(i,j,k) != 0) {
                    //if (chunk->getVoxelType(i,j+1,k) == 0) {
                    //    chunk->setVoxelType(i,j,k,2);
                    //}
                }
            }
        }
    }


}

}
