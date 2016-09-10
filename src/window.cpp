/*
 * =====================================================================================
 *
 *       Filename:  window.cpp
 *
 *    Description:  SDL window wrapper
 *
 *        Version:  1.0
 *        Created:  03/24/2014 11:15:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "window.h"

#include <glm/gtc/matrix_transform.hpp>

namespace vtk {

bool Window::create() {
    window = SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              resX, resY, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);
    return true;

}

void Window::setTitle(const std::string& title) {
    this->title = title;
}

void Window::setResolution(const int& x, const int& y) {
    resX = x;
    resY = y;
}

std::pair<int, int> Window::getResolution() {
    std::pair<int, int> resolution;
    SDL_GetWindowSize(window, &resolution.first, &resolution.second);
    return resolution;
}

float Window::getAspect() {
    return (float)resX / (float)resY;
}

SDL_Window* Window::getWindow() {
    return window;
}

void Window::setFOV(const float& angle) {
    fov = angle;
}

glm::mat4 Window::getProjectionMatrix() {
    return glm::perspective(glm::radians(fov), getAspect(), 0.1f, 500.0f);
}

}
