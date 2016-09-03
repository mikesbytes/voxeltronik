/*
 * =====================================================================================
 *
 *       Filename:  voxelmath.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/07/2014 03:48:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "voxelmath.h"
#include "world.h"
#include "mathplus.h"

#include <cmath>
#include <iostream>

namespace vtk {

void VoxelMath::rayCast(glm::vec3& hitPos, glm::vec3& hitNormal, bool& success, 
                        const glm::vec3& origin, const glm::vec3& direction, const float& radius) {

    glm::vec3 adjustedOrigin = origin * linkedWorld->voxelSize;
    glm::vec3 voxelLoc(floor(adjustedOrigin.x),
                       floor(adjustedOrigin.y),
                       floor(adjustedOrigin.z));

    glm::vec3 step((float)mp::signum(direction.x),
                   (float)mp::signum(direction.y),
                   (float)mp::signum(direction.z));

    glm::vec3 max(mp::bound(adjustedOrigin.x, direction.x),
                  mp::bound(adjustedOrigin.y, direction.y),
                  mp::bound(adjustedOrigin.z, direction.z));

    glm::vec3 delta(step.x / direction.x,
                    step.y / direction.y,
                    step.z / direction.z);

    float nRadius = radius / sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);

    if (direction.x == 0 && direction.y == 0 && direction.z == 0) {
        success = false;
        return;
    }

    while (true) {
        //std::cout << voxelLoc.x << ", " << voxelLoc.y << ", " << voxelLoc.z << std::endl;
        //std::cout << hitNormal.x << ", " << hitNormal.y << ", " << hitNormal.z << std::endl;

        if (max.x < max.y) {
            if (max.x < max.z) {
                if (max.x > nRadius) { 
                    success = false; 
                    break;
                }

                voxelLoc.x += step.x;
                max.x += delta.x;

                hitNormal.x = -step.x;
                hitNormal.y = 0.0f;
                hitNormal.z = 0.0f;

            } else {
                if (max.z > nRadius) { 
                    success = false; 
                    break;
                }
                
                voxelLoc.z += step.z;
                max.z += delta.z;

                hitNormal.x = 0.0f;
                hitNormal.y = 0.0f;
                hitNormal.z = -step.z;
            }
        } else {
            if (max.y < max.z) {
                if (max.y > nRadius) {
                    success = false;
                    break;
                }
                
                voxelLoc.y += step.y;
                max.y += delta.y;

                hitNormal.x = 0.0f;
                hitNormal.y = -step.y;
                hitNormal.z = 0.0f;
            } else {
                if (max.z > nRadius) { 
                    success = false; 
                    break;
                }
                
                voxelLoc.z += step.z;
                max.z += delta.z;

                hitNormal.x = 0.0f;
                hitNormal.y = 0.0f;
                hitNormal.z = -step.z;
            }
        }

        if (linkedWorld->isVoxelSolid((int)voxelLoc.x, (int)voxelLoc.y, (int)voxelLoc.z)) {
            success = true;
            hitPos = voxelLoc;
            break;
        }
    }
    
}

iPos VoxelMath::getChunkContaining(const int& x, const int& y, const int& z) {
    return std::make_tuple(floor((float)x / (float)linkedWorld->chunkSize), 
                           floor((float)y / (float)linkedWorld->chunkSize), 
                           floor((float)z / (float)linkedWorld->chunkSize));
}

}
