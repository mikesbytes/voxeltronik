/*
 * =====================================================================================
 *
 *       Filename:  window.h
 *
 *    Description:  SDL window wrapper
 *
 *        Version:  1.0
 *        Created:  03/24/2014 11:14:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <utility>
#include <glm/glm.hpp>

#include "graphics/rendertarget.h"

namespace vtk {

class Window : public RenderTarget {
public:
	void activate();
    bool create();
    void setTitle(const std::string& title);
    void setResolution(const int& x, const int& y);
    std::pair<int, int> getResolution();
    void setFOV(const float& angle);
    glm::mat4 getProjectionMatrix();
    SDL_Window* getWindow();

protected:
    
    float fov;

    std::string title;
    SDL_Window* window;
    SDL_GLContext context;
};

}
