#include "graphics/drawable.h"
#include "sol.hpp"

namespace vtk {

Drawable::~Drawable() {

}

void Drawable::setShader(std::shared_ptr<Shader> shader) {
	mLinkedShader = shader;
}

std::shared_ptr<Shader> Drawable::getShader() {
	return mLinkedShader;

}

void Drawable::registerScriptInterface(::sol::state &lua) {
	lua.new_usertype<Drawable>("Drawable");
}

}
