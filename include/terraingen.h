#pragma once

#include <map>
#include <utility>
#include <memory>

#include "terrain/noisemodule.h"

namespace vtk {

class Chunk;

class TerrainGen {
public:
    TerrainGen();

    void generateChunk(Chunk* chunk);

protected:
	std::shared_ptr<noise::NoiseModule> mNoise;

    //variables (replace by config)
    double mTerrainScale;
};

}
