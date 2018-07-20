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
