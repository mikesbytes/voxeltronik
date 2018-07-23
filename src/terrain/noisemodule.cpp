#include "terrain/noisemodule.h"
#include "sol.hpp"

namespace vtk { namespace noise {

void NoiseModule::registerScriptInterface(::sol::state &lua) {
	lua.new_usertype<NoiseModule>("NoiseModule");
}

}}
