/*
 * =====================================================================================
 *
 *       Filename:  voxelmath.h
 *
 *    Description:  Voxel math class used in world
 *
 *        Version:  1.0
 *        Created:  04/07/2014 03:44:24 PM
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

#include <glm/glm.hpp>

namespace vtk {

class World;

class VoxelMath {
public:

    void rayCast(glm::vec3& hitPos, glm::vec3& hitNormal, bool& success, 
                 const glm::vec3& origin, const glm::vec3& direction, const float& radius);

    iPos getChunkContaining(const int& x, const int& y, const int& z);



    World* linkedWorld;

protected:

};

}
