/*
 * =====================================================================================
 *
 *       Filename:  game.h
 *
 *    Description:  Main game class
 *
 *        Version:  1.0
 *        Created:  03/23/2014 06:29:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "window.h"
#include "config.h"

namespace vtk {

class Scene;

class Game {
public:

    Game();
    void init();
    void start();
    void loop();
    void stop();
    void setScene(Scene* scene);
    void setConfig(Config* conf);
    Config* getConfig();
    
    Window window;
protected:
    Config* conf;
    Scene* activeScene;
    void cleanup();
    bool running;
};

}
