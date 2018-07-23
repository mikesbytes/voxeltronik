#include "terrain/decorator.h"

#include "sol.hpp"

namespace vtk { namespace noise {

void Decorator::registerScriptInterface(::sol::state &lua) {
	lua.new_usertype<Decorator>("Decorator");
}

}}
