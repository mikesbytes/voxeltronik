#pragma once


#include <SDL2/SDL.h>
#include <memory>

#include "sol.hpp"


namespace vtk {

class Game;

class Scene {
public:
    void link(Game* game);
    void init(); // Called first time the scene is set
    void reInit(); // Called when switching from another scene
    void update(const float& dTime); //Logic
    void draw(); //Graphics

	//set lua functions for game stuff
	void setInitFn(sol::function f);
	void setReInitFn(sol::function f);
	void setUpdateFn(sol::function f);
	void setDrawFn(sol::function f);

	
	static void registerScriptInterface(sol::state& lua);
	static std::shared_ptr<Scene> newScene();
protected:
	sol::function mInitFn;
	sol::function mReInitFn;
	sol::function mUpdateFn;
	sol::function mDrawFn;
    Game* linkedGame;
};

}
