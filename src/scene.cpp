#include <stdlib.h>
#include <iostream>

#include "scene.h"
#include "game.h"
#include "sol.hpp"

namespace vtk {

void Scene::link(Game* game) {
    linkedGame = game;
}

void Scene::init() {
	mInitFn();
}

void Scene::reInit() {
	mReInitFn();
}

void Scene::update(const float &dTime) {
	mUpdateFn(dTime);
}

void Scene::draw() {
	mDrawFn();
}

void Scene::setInitFn(sol::function f) {
	mInitFn = f;
}

void Scene::setReInitFn(sol::function f) {
	mReInitFn = f;
}

void Scene::setUpdateFn(sol::function f) {
	mUpdateFn = f;
}

void Scene::setDrawFn(sol::function f) {
	mDrawFn = f;
}

void Scene::registerScriptInterface(::sol::state &lua) {
	auto factories = sol::factories(&Scene::newScene);
	lua.new_usertype<Scene>("Scene",
	                        "set_init_fn", &Scene::setInitFn,
	                        "set_reinit_fn", &Scene::setReInitFn,
	                        "set_update_fn", &Scene::setUpdateFn,
	                        "set_draw_fn", &Scene::setDrawFn,
	                        sol::meta_function::construct, factories,
	                        sol::call_constructor, factories);
}

std::shared_ptr<Scene> Scene::newScene() {
	return std::make_shared<Scene>();
}

}
