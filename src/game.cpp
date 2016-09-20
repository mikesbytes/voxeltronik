/*
 * =====================================================================================
 *
 *       Filename:  game.cpp
 *
 *    Description:  Game class source
 *
 *        Version:  1.0
 *        Created:  03/23/2014 11:33:25 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "game.h"
#include "loadShader.h"
#include "graphics/camera.h"
#include "graphics/glstate.h"
#include "scene.h"

//SPAGHETTI INCLUDES
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace vtk {

Game::Game() {
    activeScene = nullptr;
}

void Game::init() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetSwapInterval(conf->getValue<int>("graphics.vsync", 0));

    window.setTitle("OpenGL");
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

    running = false;
	gls::setTracking(true);
}

void Game::start() {
    if (activeScene == nullptr) return;
    running = true;
    loop();
}

void Game::loop() {


    int lastFrameTime = SDL_GetTicks();

    while (running) {
        //time
        int dTimeMS = SDL_GetTicks() - lastFrameTime;
        lastFrameTime = SDL_GetTicks();
        float dTime = (float)dTimeMS / 1000.0f;

        activeScene->update(dTime);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        activeScene->draw();

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

void Game::setScene(Scene* scene) {
    activeScene = scene;
    scene->link(this);
    scene->init();
}

void Game::setConfig(Config* conf) {
    this->conf = conf;
}

Config* Game::getConfig() {
    return conf;
}

};
