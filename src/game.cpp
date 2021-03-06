#include "game.h"
#include "loadShader.h"
#include "graphics/camera.h"
#include "graphics/glstate.h"
#include "scene.h"
#include "scenes/scriptscene.h"
#include "sol.hpp"

//SPAGHETTI INCLUDES
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

namespace vtk {

Game::Game() {
    activeScene = nullptr;
}

void Game::init() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetSwapInterval(conf->getValue<int>("graphics.vsync", 0));

    window.setTitle("VTK");
    window.setResolution(conf->getValue<int>("graphics.res.x", 800),
                         conf->getValue<int>("graphics.res.y", 600));
    window.setFOV(conf->getValue<float>("graphics.fov", 45.0f));

    window.create();

    glewExperimental = GL_TRUE;
    glewInit();

    //OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    //needed for inverted depth buffer for long distance rendering
    glDepthFunc(GL_GREATER);
    glClearDepth(0.0f);
    glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);


    running = false;
    gls::setTracking(true); // track OpenGL state changes
}

void Game::start() {
    if (activeScene == nullptr) return;
    running = true;
    loop();
}

void Game::loop() {


	std::chrono::steady_clock::time_point lastFrameTime = std::chrono::steady_clock::now();

    while (running) {
		//time
		std::chrono::steady_clock::time_point curTime = std::chrono::steady_clock::now();
		auto timeSpan = std::chrono::duration_cast<std::chrono::duration<float>>(curTime - lastFrameTime);
        lastFrameTime = curTime;
        float dTime = timeSpan.count();

        activeScene->update(dTime);
        
        //glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		glDepthFunc(GL_GREATER);
		glClearDepth(0.0f);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        activeScene->draw(); //draw the scene
        
        SDL_GL_SwapWindow(window.getWindow());
    }
    cleanup();
}

void Game::stop() {
    running = false;
}

void Game::cleanup() {
    SDL_Quit();
}

void Game::setScene(std::shared_ptr<Scene> scene) {
    activeScene = scene;
    scene->link(this);
    scene->init();
}

void Game::setScriptScene(ScriptScene *scene) {
	//setScene((Scene*)scene);
}


void Game::setConfig(Config* conf) {
    this->conf = conf;
}

Config* Game::getConfig() {
    return conf;
}

Window& Game::getWindow() {
	return window;
}

void Game::registerScriptInterface(sol::state &lua) {
	lua.new_usertype<Game>("Game",
	                       "set_config", &Game::setConfig,
	                       "init", &Game::init,
	                       "set_scene", &Game::setScene,
	                       "start", &Game::start,
	                       "stop", &Game::stop,
	                       "get_window", &Game::getWindow);
}

};
