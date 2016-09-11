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

#include <noise/noise.h>
#include <map>
#include <utility>


namespace vtk {

class Chunk;

class TerrainGen {
public:
    TerrainGen();

    void generateChunk(Chunk* chunk);

    //libnoise modules
    noise::module::Select selectorMod;
    noise::module::Const const1Mod;
    noise::module::Const const0Mod;
    noise::module::Const constNeg1Mod;
    noise::module::Perlin terrainShape;

    //variables (replace by config)
    double terrainScale;
};

}
