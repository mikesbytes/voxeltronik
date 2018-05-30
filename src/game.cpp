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

    //needed for inverted depth buffer for long distance rendering
    glDepthFunc(GL_GREATER);
    glClearDepth(0.0f);
    glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);

    //depth buffer stuff
    glGenTextures(1, &mColor);
    glBindTexture(GL_TEXTURE_2D, mColor);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_SRGB8_ALPHA8, conf->getValue<int>("graphics.res.x", 800), conf->getValue<int>("graphics.res.y", 600));
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &mDepth);
    glBindTexture(GL_TEXTURE_2D, mDepth);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, conf->getValue<int>("graphics.res.x", 800), conf->getValue<int>("graphics.res.y", 600));
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColor, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepth, 0);
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "glCheckFramebufferStatus: %x\n", status);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
        

        glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
		glDepthFunc(GL_GREATER);
		glClearDepth(0.0f);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        activeScene->draw(); //draw the scene
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
		glBindFramebuffer(GL_READ_FRAMEBUFFER, mFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // default FBO
		glBlitFramebuffer(
			0, 0, conf->getValue<int>("graphics.res.x", 800), conf->getValue<int>("graphics.res.y", 600),
			0, 0, conf->getValue<int>("graphics.res.x", 800), conf->getValue<int>("graphics.res.y", 600),
			GL_COLOR_BUFFER_BIT, GL_LINEAR);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
