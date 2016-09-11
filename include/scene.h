/*
 * =====================================================================================
 *
 *       Filename:  scene.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/24/2014 04:10:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#pragma once

#include <SDL2/SDL.h>

namespace vtk {

class Game;

class Scene {
public:
    void link(Game* game);
    virtual void init() = 0; // Called first time the scene is set
    virtual void reInit() = 0; // Called when switching from another scene
    virtual void update(const float& dTime) = 0; //Logic
    virtual void draw() = 0; //Graphics


protected:
    Game* linkedGame;
};

}
