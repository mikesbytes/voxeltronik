#include "raycast.h"

#include "world.h"
#include "mathplus.h"
#include "sol.hpp"

namespace vtk {

RayCast::RayCast(World& world) :
	mWorld(world)
{

}

std::tuple<bool, glm::vec3, glm::vec3> RayCast::cast(const glm::vec3& origin, const glm::vec3& direction, const float& radius) {
	bool success;
	glm::vec3 hitPos;
	glm::vec3 hitNormal;


    glm::vec3 adjustedOrigin = origin * mWorld.voxelSize;
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
        return std::make_tuple(success,hitPos,hitNormal);
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

        if (mWorld.isVoxelSolid((int)voxelLoc.x, (int)voxelLoc.y, (int)voxelLoc.z)) {
            success = true;
            hitPos = voxelLoc;
            break;
        }
    }
	return std::make_tuple(success,hitPos,hitNormal);
}

void RayCast::registerScriptInterface(::sol::state &lua) {
	lua.new_usertype<RayCast>("RayCast",
	                          sol::constructors<RayCast(World&)>(),
	                          "cast", &RayCast::cast);
}

}
