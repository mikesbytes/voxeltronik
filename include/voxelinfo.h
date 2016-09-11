/*
 * =====================================================================================
 *
 *       Filename:  voxelinfo.h
 *
 *    Description:  Stores information about voxel types, like texture coordinates.
 *
 *        Version:  1.0
 *        Created:  04/06/2014 01:07:52 PM
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

#include <map>
#include <vector>
#include <glm/glm.hpp>

namespace vtk {

enum class Corner2D {
    TOPLEFT,
    TOPRIGHT,
    BOTTOMLEFT,
    BOTTOMRIGHT
};

enum class Face3D {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    FRONT,
    BACK
};

enum class Orientation2D {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct VoxelData{
    std::map<Face3D, float> faceTextureIndices;
    std::map<Face3D, Orientation2D> faceOrientation;

    bool transparent;
};

class World;

class VoxelInfo {
public:
    void pushTexCoordFromWorldCoords(std::vector<float>& data, const glm::ivec3& pos, const Face3D& face, const Corner2D& corner);
    void pushTexCoordFromVoxelID(std::vector<float>& data, const unsigned& id, const Face3D& face, const Corner2D& corner);
    float getTexIndexFromID(const unsigned& id, const Face3D& face);
    
    void setTextureData(const unsigned& id, const Face3D& face, const Orientation2D& orientation, const float& textureIndex);

    void setTransparent(const unsigned& id, const bool& transparent);
    bool isTransparent(const unsigned& id);

    std::pair<float, float> getTexCoordFromID(const unsigned& id, const Face3D& face, const Corner2D& corner);

    World* linkedWorld;

protected:

    std::map<unsigned, VoxelData> voxelDataMap;
};

}
