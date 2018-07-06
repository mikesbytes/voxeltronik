#include "graphics/rendertarget.h"
#include "sol.hpp"

namespace vtk {

double RenderTarget::getAspect() {
	return (double)mWidth / (double)mHeight;
}

int RenderTarget::width() {
	return mWidth;
}

int RenderTarget::height() {
	return mHeight;
}

void RenderTarget::registerScriptInterface(::sol::state &lua) {
	lua.new_usertype<RenderTarget>("RenderTarget",
	                               "get_aspect", &RenderTarget::getAspect,
	                               "width", &RenderTarget::width,
	                               "height", &RenderTarget::height);
}

}
