#include "window.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GL/gl.h>

#include "sol.hpp"

namespace vtk {

void Window::activate() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool Window::create() {
    window = SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              mWidth, mHeight, SDL_WINDOW_OPENGL);
    
    context = SDL_GL_CreateContext(window);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
    return true;

}

void Window::setTitle(const std::string& title) {
    this->title = title;
}

void Window::setResolution(const int& x, const int& y) {
    mWidth = x;
    mHeight = y;
}

std::pair<int, int> Window::getResolution() {
    std::pair<int, int> resolution;
    SDL_GetWindowSize(window, &resolution.first, &resolution.second);
    return resolution;
}


SDL_Window* Window::getWindow() {
    return window;
}

void Window::setFOV(const float& angle) {
    fov = angle;
}

glm::mat4 Window::getProjectionMatrix() {
    return glm::perspective(glm::radians(fov), (float)getAspect(), 0.1f, 500.0f);
}

bool Window::saveScreenshot(const std::string& fileName) {
	SDL_Surface* surf = SDL_CreateRGBSurface(SDL_SWSURFACE, mWidth, mHeight, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);

	glReadPixels(0, 0, mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, surf->pixels);

	//image is upside down by default so invert it
	auto flipped = flipVert(surf);

	//SDL_Surface* surf = SDL_CreateRGBSurfaceFrom(pixels, mWidth, mHeight, 8*4, mWidth*4, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	if (SDL_SaveBMP(flipped, fileName.c_str()) != 0) return false;

	SDL_FreeSurface(surf);
	SDL_FreeSurface(flipped);
	return true;
}

SDL_Surface* Window::flipVert(SDL_Surface* sfc) {
    // create a copy of the image
     SDL_Surface* result = SDL_CreateRGBSurface(sfc->flags, sfc->w, sfc->h,
         sfc->format->BytesPerPixel * 8, sfc->format->Rmask, sfc->format->Gmask,
         sfc->format->Bmask, sfc->format->Amask);
     unsigned char* pixels = static_cast<unsigned char*>(sfc->pixels);
     unsigned char* rpixels = static_cast<unsigned char*>(result->pixels);
     int pitch = sfc->pitch;
     int pxlength = pitch*sfc->h;

     for(int line = 0; line < sfc->h; ++line) {
         int pos = line * pitch;
         memcpy(rpixels + pos, pixels + (pxlength - pos - pitch), pitch);
     }
     return result;
}

void Window::registerScriptInterface(::sol::state &lua) {
	lua.new_usertype<Window>("Window",
	                         "get_aspect_ratio", &Window::getAspect,
	                         "save_screenshot", &Window::saveScreenshot,
	                         sol::base_classes, sol::bases<RenderTarget>());
}

}
