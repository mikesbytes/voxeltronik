#pragma once

#include <glm/glm.hpp>

namespace vtk {

class HeightMap {
public:
	HeightMap(const glm::ivec2& pos);

protected:
	glm::ivec2 mPos;
};

}
