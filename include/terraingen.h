#pragma once

#include <map>
#include <utility>
#include <memory>

#include "terrain/decorator.h"

#include "sol.hpp"

namespace vtk {

class Chunk;

class TerrainGen {
public:
    TerrainGen();

    void generateChunk(Chunk* chunk);
	void setDecorator(noise::Decorator* dec);

	static void registerScriptInterface(sol::state& lua);
protected:
	noise::Decorator* mDecorator;

    //variables (replace by config)
    double mTerrainScale;
};

}
