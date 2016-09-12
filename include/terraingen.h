/*
 * =====================================================================================
 *
 *       Filename:  terraingen.h
 *
 *    Description:  Terrain generator powered by libnoise
 *
 *        Version:  1.0
 *        Created:  05/08/2014 06:42:22 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include <map>
#include <utility>

#include "terrain/noisemodule.h"

namespace vtk {

class Chunk;

class TerrainGen {
public:
    TerrainGen();

    void generateChunk(Chunk* chunk);

protected:
	NoiseModule* mNoise;

    //variables (replace by config)
    double mTerrainScale;
};

}
