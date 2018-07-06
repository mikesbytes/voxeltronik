#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <memory>

#include "window.h"
#include "config.h"

namespace sol {
class state;
}

namespace vtk {

class ScriptScene;
class Scene;

class Game {
public:

    Game();
    void init();
    void start();
    void loop();
    void stop();
	void setScene(std::shared_ptr<Scene> scene);
	void setScriptScene(ScriptScene* scene);
    void setConfig(Config* conf);
    Config* getConfig();

	Window& getWindow();

	static void registerScriptInterface(sol::state& lua);
    
    Window window;
protected:
    Config* conf;
	std::shared_ptr<Scene> activeScene;
    void cleanup();
    bool running;

	GLuint mColor, mDepth, mFBO;
};

}
