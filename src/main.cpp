#include "game.h"
#include "scenes/testscene.h"
#include "scenes/scriptscene.h"
#include "config.h"
#include "mathplus.h"
#include "spdlog/spdlog.h"
#include "loggersetup.h"
#include "threadpool.h"
#include "sol.hpp"

#include <iostream>
#include <vector>
#include <memory>


int main (int argc, char *argv[])
{
	vtk::LoggerSetup lSetup;
	lSetup.setup();
	spdlog::get("general")->info("Starting Voxeltronik Engine...");
	spdlog::get("general");

	vtk::ThreadPool::getInstance().addThreads(2);

	auto conf = new Config;
	conf->loadConfigFromFile("res/config.conf");
	conf->addArgumentRule("-w", "graphics.res.x");
	conf->addArgumentRule("-h", "graphics.res.y");
	conf->loadConfigFromArguments(argc, argv);

	sol::state lua;
	lua.open_libraries(sol::lib::base, sol::lib::package);

	lua.new_usertype<glm::mat4>("mat4");
	lua.new_usertype<glm::vec3>("vec3");
	lua.new_usertype<glm::ivec3>("ivec3");
	
	vtk::LoggerSetup::registerScriptInterface(lua);
	Config::registerScriptInterface(lua);
	vtk::Scene::registerScriptInterface(lua);
	vtk::Game::registerScriptInterface(lua);
	vtk::Camera::registerScriptInterface(lua);
	vtk::Drawable::registerScriptInterface(lua);
	vtk::RenderTarget::registerScriptInterface(lua);
	vtk::Window::registerScriptInterface(lua);
	vtk::RenderTask::registerScriptInterface(lua);
	vtk::Skybox::registerScriptInterface(lua);

	lua.script_file("res/init.lua");

	/*
	vtk::Game game;
	game.setConfig(conf);
	game.init();
	game.setScene(new vtk::TestScene);
	game.start();
	*/

	return 0;
}
