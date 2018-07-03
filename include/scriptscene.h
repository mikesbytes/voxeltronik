#pragma once

#include "scene.h"

namespace vtk {

class ScriptScene : public Scene {
public:
    void init(); // Called first time the scene is set
    void reInit(); // Called when switching from another scene
    void update(const float& dTime); //Logic
    void draw(); //Graphics
};

}
