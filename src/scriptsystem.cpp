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
#include "terraingen.h"
#include "terrain/open_simplex_noise.h"
#include "terrain/noisemodule.h"
#include "terrain/noise.h"
#include "terrain/decorator.h"
#include "terrain/binarydecorator.h"
#include "terrain/ygradient.h"

#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

#include <GL/glew.h>
#include <GL/gl.h>

namespace vtk {

struct WrappedNVGcontext {
	std::unique_ptr<NVGcontext> ctx;
};

struct NVGDeleter {
	void operator()( NVGcontext* nvgcontext ) {
		     nvgDeleteGL3(nvgcontext);
    }
};

template <typename T>
void freak_out_if_called(T*) {
    assert("YOU SCREWED UP, FAM");
}

NVGcontext* createNVGcontext() {
	return nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
}

ScriptSystem::ScriptSystem() {
	mLua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::package);
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
	                              sol::constructors<glm::ivec3(), glm::ivec3(int, int, int), glm::ivec3(glm::vec3)>(),
								"x", &glm::ivec3::x,
								"y", &glm::ivec3::y,
								"z", &glm::ivec3::z
								);
	mLua.new_usertype<glm::ivec2>("ivec2",
	                              sol::constructors<glm::ivec2(), glm::ivec2(int, int)>(),
								"x", &glm::ivec2::x,
								"y", &glm::ivec2::y
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
	TerrainGen::registerScriptInterface(mLua);
	noise::NoiseModule::registerScriptInterface(mLua);
	noise::Noise::registerScriptInterface(mLua);
	noise::YGradient::registerScriptInterface(mLua);
	noise::Decorator::registerScriptInterface(mLua);
	noise::BinaryDecorator::registerScriptInterface(mLua);

	//utilities
	mLua["FaceDirection"] = mLua.create_table_with("TOP", FaceDirection::TOP,
	                                               "BOTTOM", FaceDirection::BOTTOM,
	                                               "NORTH", FaceDirection::NORTH,
	                                               "SOUTH", FaceDirection::SOUTH,
	                                               "EAST", FaceDirection::EAST,
	                                               "WEST", FaceDirection::WEST);
	mLua.set_function("RGBD", [](unsigned char r,
	                             unsigned char g,
	                             unsigned char b,
	                             unsigned char d) {
		                          unsigned char light = r << 12;
		                          light = light | g << 8;
		                          light = light | b << 4;
		                          light = light | d;
		                          return light;
	                          });
	
	//nanovg
	auto nvgTable = mLua.create_named_table("NVG");
	nvgTable.set_function(	
	                      "create", [](){return std::unique_ptr<NVGcontext, NVGDeleter>
			                      (nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES));}
		);
	nvgTable.set_function("create_font", &nvgCreateFont);
	nvgTable.set_function("rgba", &nvgRGBA);
	nvgTable.set_function("begin_frame", &nvgBeginFrame);
	nvgTable.set_function("font_size", &nvgFontSize);
	nvgTable.set_function("font_face", &nvgFontFace);
	nvgTable.set_function("fill_color", &nvgFillColor);
	nvgTable.set_function("text", [](NVGcontext* vg, const float& x, const float& y, const std::string& text){
		                              nvgText(vg, x, y, text.data(), NULL);});
	nvgTable.set_function("begin_path", &nvgBeginPath);
	nvgTable.set_function("rect", &nvgRect);
	nvgTable.set_function("fill", &nvgFill);
	nvgTable.set_function("end_frame", &nvgEndFrame);

	//TODO: nanovg metatable madness

	//OpenGL
	mLua["GL"] = mLua.create_table_with("CULL_FACE", GL_CULL_FACE,
	                                    "DEPTH_TEST", GL_DEPTH_TEST,
	                                    "CW", GL_CW,
	                                    "CCW", GL_CCW);
	sol::table glTable = mLua["GL"];
	glTable.set_function("enable", &glEnable);
	glTable.set_function("front_face", &glFrontFace);
}

void ScriptSystem::runScript(const std::string &fileName) {
	mLua.script_file(fileName);
}

}
