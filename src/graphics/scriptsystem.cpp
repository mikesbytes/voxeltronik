#include "scriptsystem.h"

#include <glm/glm.hpp>

#include "loggersetup.h"
#include "config.h"
#include "scene.h"
#include "game.h"
#include "graphics/camera.h"
#include "graphics/drawable.h"
#include "graphics/rendertask.h"
#include "graphics/skybox.h"
#include "tileset.h"
#include "inputhandler.h"
#include "voxelutils.h"
#include "voxelinfo.h"
#include "world.h"
#include "raycast.h"


namespace vtk {

ScriptSystem::ScriptSystem() {
	mLua.open_libraries(sol::lib::base, sol::lib::package);
}

void ScriptSystem::registerAllInterfaces() {
	//register glm types
	mLua.new_usertype<glm::mat4>("mat4");
	mLua.new_usertype<glm::vec3>("vec3",
	                             sol::constructors<glm::vec3(), glm::vec3(float, float, float)>(),
	                             "x", &glm::vec3::x,
	                             "y", &glm::vec3::y,
	                             "z", &glm::vec3::z
	                             );
	mLua.new_usertype<glm::ivec3>("ivec3",
	                              sol::constructors<glm::ivec3(), glm::ivec3(int, int, int)>(),
								"x", &glm::ivec3::x,
								"y", &glm::ivec3::y,
								"z", &glm::ivec3::z
								);

	//SDL functions
	mLua["SDL_EventType"] = mLua.create_table_with("SDL_QUIT", SDL_QUIT,
	                                               "SDL_MOUSEMOTION", SDL_MOUSEMOTION
	                                               );
	mLua.set_function("sdl_relative_mouse_mode", &SDL_SetRelativeMouseMode);
	mLua.set_function("sdl_get_relative_mouse_state", []() {
		                                                  int x, y;
		                                                  SDL_GetRelativeMouseState(&x, &y);
		                                                  return std::make_tuple(x,y);
	                                                  });

	//register vtk types
	vtk::LoggerSetup::registerScriptInterface(mLua);
	Config::registerScriptInterface(mLua);
	vtk::Scene::registerScriptInterface(mLua);
	vtk::Game::registerScriptInterface(mLua);
	vtk::Camera::registerScriptInterface(mLua);
	vtk::Drawable::registerScriptInterface(mLua);
	vtk::RenderTarget::registerScriptInterface(mLua);
	vtk::Window::registerScriptInterface(mLua);
	vtk::RenderTask::registerScriptInterface(mLua);
	vtk::Skybox::registerScriptInterface(mLua);
	vtk::InputHandler::registerScriptInterface(mLua);
	vtk::Tileset::registerScriptInterface(mLua);
	vtk::VoxelInfo::registerScriptInterface(mLua);
	vtk::World::registerScriptInterface(mLua);
	RayCast::registerScriptInterface(mLua);

	//utilities
	mLua["FaceDirection"] = mLua.create_table_with("TOP", FaceDirection::TOP,
	                                               "BOTTOM", FaceDirection::BOTTOM,
	                                               "NORTH", FaceDirection::NORTH,
	                                               "SOUTH", FaceDirection::SOUTH,
	                                               "EAST", FaceDirection::EAST,
	                                               "WEST", FaceDirection::WEST);
}

void ScriptSystem::runScript(const std::string &fileName) {
	mLua.script_file(fileName);
}

}
