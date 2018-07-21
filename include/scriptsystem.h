#pragma once

#include <string>

#include "nanovg.h"
#include "sol.hpp"

namespace sol {
template <>
struct is_automagical<NVGcontext> : std::false_type {};
}

namespace vtk {

class ScriptSystem {
public:
	ScriptSystem();

	void registerAllInterfaces();
	void runScript(const std::string& fileName);
protected:
	sol::state mLua;
};

}
