#pragma once

#include "scene.h"


//forward declaration
namespace sol {
class state;
}


namespace vtk {
class ScriptScene : public Scene {
public:
    void init(); // Called first time the scene is set
    void reInit(); // Called when switching from another scene
    void update(const float& dTime); //Logic
    void draw(); //Graphics

	static void registerScriptInterface(::sol::state& lua);
};

}
