/*
 * Handles geometry for individual chunks
 */

#pragma once

#include "graphics/drawable.h"

namespace vtk {

class Chunk;

class ChunkGeometry : public Drawable {
public:
	void draw(RenderTask& task);
	void buildGeometry(Chunk& chunk);
	void buildVertices(Chunk& chunk);
	void buildLighting(Chunk& chunk);
protected:
	
};

}
