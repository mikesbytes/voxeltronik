/*
 * =====================================================================================
 *
 *       Filename:  chunkrenderer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/04/2014 11:11:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include "voxelutils.h"

#include <array>
#include <vector>
#include <atomic>

namespace vtk {

class Chunk;

class ChunkRenderer {
public:

    void init();

    void drawChunk();
    void setChunkPosition(const int&x, const int& y, const int&z);

    void updateGeometry();
    void updateTexCoords();
    void updateLighting();

    void updateVertexData();

    Chunk* linkedChunk;
protected:

    /*
        Info I need to reference:
        0 = X+
        1 = X-
        2 = y+
        3 = Y-
        4 = Z+
        5 = Z-
    */

    bool initialized;
    iPos chunkPos;
    float voxelSize;
    unsigned chunkSize;
    unsigned lightingMethod; // 0 = fullbright, 1 = blocky, 2 = smooth

    bool updatingGeometry; //Is geometry actively updating (for threading)

    //set to true after their respective update function is called
    bool hasGeometryChanged;
    bool hasTexturesChanged;
    bool hasLightingChanged;


    std::array<unsigned, 6> faceCounts;
    std::array<unsigned, 6> vertexCounts;

    std::array<std::vector<float>,  6> geometryVertices; //Holds geometry vertices as x,y,z,x,y,z...
    std::array<std::vector<float>,  6> textureCoords;    //Holds texture coords as u,v,i,u,v,i...
    std::array<std::vector<float>,  6> lightingData;     //Holds lighting data as r,g,b,r,g,b...

    std::array<std::array <unsigned, 3>, 6> vertexBuffers;//VBOs
    std::array<unsigned, 6> vertexArrays;       //VAOs

    //add vertex to a vector of floats. Used for code cleanliness and flexibility
    void addVertex(std::vector<float>& data, const float& x, const float& y, const float& z);
};

}
