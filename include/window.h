#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <utility>
#include <glm/glm.hpp>

#include "graphics/rendertarget.h"

namespace sol {
class state;
}

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
	bool saveScreenshot(const std::string& fileName);

	static void registerScriptInterface(::sol::state& lua);

protected:

	SDL_Surface* flipVert(SDL_Surface* sfc);
	
    float fov;

    std::string title;
    SDL_Window* window;
    SDL_GLContext context;
};

}
