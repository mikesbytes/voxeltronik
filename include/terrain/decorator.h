#pragma once

#include <glm/glm.hpp>

namespace sol {
class state;
}

namespace vtk { namespace noise {

class Decorator {
public:
	virtual unsigned get3D(const glm::ivec3& pos) { return 0; }
	static void registerScriptInterface(::sol::state& lua);
protected:
};

}}
