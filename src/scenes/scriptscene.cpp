#include "scenes/scriptscene.h"
#include "sol.hpp"

namespace vtk {

void ScriptScene::init() {

}

void ScriptScene::reInit() {

}

void ScriptScene::update(const float& dTime) {

}

void ScriptScene::draw() {

}

void ScriptScene::registerScriptInterface(::sol::state& lua) {
	lua.new_usertype<ScriptScene>("scene", sol::base_classes, sol::bases<Scene>());
}


}

