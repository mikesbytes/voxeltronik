#pragma once

#include "scene.h"
#include "graphics/camera.h"
#include "inputhandler.h"
#include "world.h"
#include "graphics/skybox.h"
#include "graphics/shader.h"

#include "nanovg.h"

#include <SDL2/SDL.h>

namespace vtk {

class Game;

class TestScene : public Scene {
public:
    void init(); // Called first time the scene is set
    void reInit(); // Called when switching from another scene
    void update(const float& dTime); //Logic
    void draw(); //Graphics

    //Game functions
    void look(); //look with camera

protected:
    unsigned vao;
    unsigned shaders;
    float dTime;
    float mFPS;
    Camera camera;
    SDL_Event event;
    World world;
	Skybox mSkybox;
	RenderTask* mSkyboxTask;
	NVGcontext* vg; //nanovg context

    //stuff
    float sensitivity;
    glm::vec3 camMovement;
	glm::vec3 mCamLastLoadPosition;
    bool placeVoxel;
    unsigned voxelType;

	bool mNoclip;
	bool mNoclipDebounce;
	glm::vec3 mVelocity;

    //controls
    InputHandler handler;

    //shader uniforms
    int viewMatUni;
    int projMatUni;
    int modelMatUni;
};
}
