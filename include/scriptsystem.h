#pragma once

#include <string>

#include "sol.hpp"

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
