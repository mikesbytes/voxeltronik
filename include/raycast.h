#pragma once

#include <tuple>
#include <glm/glm.hpp>

namespace sol {
class state;
}

namespace vtk {

class World;

class RayCast {
public:
	RayCast(World& world);

	//returns (success, hitpos, hitnormal)
	std::tuple<bool, glm::vec3, glm::vec3> cast(const glm::vec3& origin, const glm::vec3& direction, const float& radius);

	static void registerScriptInterface(::sol::state& lua);

protected:
	World& mWorld;
};
}
