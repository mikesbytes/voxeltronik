/*
 * =====================================================================================
 *
 *       Filename:  testscene.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/24/2014 11:06:32 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "testscene.h"
#include "loadShader.h"
#include "game.h"
#include "tileset.h"
#include "world.h"
#include "mathplus.h"
#include "graphics/glstate.h"


#include <GL/glew.h>
#include <GL/gl.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace vtk {

void TestScene::init() {

    Tileset tiles;
    tiles.buildTexture();
    tiles.updateTextureAt(0, "res/stone.png");
    tiles.updateTextureAt(1, "res/dirt.png");
    tiles.updateTextureAt(2, "res/grass.png");

    //define vertices
    float vertices[] {
        -0.01f,  0.01f, 0.0f,
         0.01f, -0.01f, 0.0f,
        -0.01f, -0.01f, 0.0f,
        -0.01f,  0.01f, 0.0f,
         0.01f,  0.01f, 0.0f,
         0.01f, -0.01f, 0.0f
    };


    glGenBuffers(1, &vertVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vertVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(0);

    //shaders
	mCursorShader.loadShaderFiles("res/shaders/test.vert.glsl", "res/shaders/test.frag.glsl");
    cursorShaders = LoadShaders("res/shaders/test.vert.glsl", "res/shaders/test.frag.glsl");
    shaders = LoadShaders("res/shaders/voxelvert.vert.glsl", "res/shaders/voxelfrag.frag.glsl");

    glUseProgram(shaders);

    camera.setPosition(glm::vec3(0.0f, 5.0f, 5.0f));
    camera.setAspectRatio(linkedGame->window.getAspect());

    viewMatUni = glGetUniformLocation(shaders, "view");
    projMatUni = glGetUniformLocation(shaders, "proj");
    modelMatUni = glGetUniformLocation(shaders, "model");
    world.modelMatUni = modelMatUni;
	mSkyboxTask = new RenderTask(mSkybox, camera, linkedGame->window);
    
    glUniformMatrix4fv(viewMatUni, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
    glUniformMatrix4fv(projMatUni, 1, GL_FALSE, glm::value_ptr(linkedGame->window.getProjectionMatrix()));

    SDL_SetRelativeMouseMode(SDL_TRUE);
    sensitivity = linkedGame->getConfig()->getValue<float>("controls.mouse.sensitivity", 5.0f);

    //get config
    Config* conf = linkedGame->getConfig();

    //add actions to handler
    handler.setAction("Move Forward",  conf->getValue<std::string>("controls.bindings.movement.forward",  "W"        ));
    handler.setAction("Move Backward", conf->getValue<std::string>("controls.bindings.movement.backward", "S"        ));
    handler.setAction("Move Left",     conf->getValue<std::string>("controls.bindings.movement.left",     "A"        ));
    handler.setAction("Move Right",    conf->getValue<std::string>("controls.bindings.movement.right",    "D"        ));
    handler.setAction("Move Up",       conf->getValue<std::string>("controls.bindings.movement.up",       "Space"    ));
    handler.setAction("Move Down",     conf->getValue<std::string>("controls.bindings.movement.down",     "Left Ctrl"));
    handler.setAction("Place Voxel",   conf->getValue<std::string>("controls.bindings.action.placevoxel", "Mouse Left"));
    handler.setAction("Delete Voxel",  conf->getValue<std::string>("controls.bindings.action.delvoxel",   "Mouse Right"));
    handler.setAction("Select Type 1", conf->getValue<std::string>("controls.bindings.typesel.type1",     "1"        ));
    handler.setAction("Select Type 2", conf->getValue<std::string>("controls.bindings.typesel.type2",     "2"        ));

    //set signals for handler
    handler.getEventSignal(SDL_QUIT       ).connect<Game     , &Game::stop     >(linkedGame);
    handler.getEventSignal(SDL_MOUSEMOTION).connect<TestScene, &TestScene::look>(this);



    world.voxelInfo.setTextureData(1, Face3D::RIGHT, Orientation2D::UP, 0.0f);
    world.voxelInfo.setTextureData(1, Face3D::LEFT, Orientation2D::UP, 0.0f);
    world.voxelInfo.setTextureData(1, Face3D::TOP, Orientation2D::UP, 0.0f);
    world.voxelInfo.setTextureData(1, Face3D::BOTTOM, Orientation2D::UP, 0.0f);
    world.voxelInfo.setTextureData(1, Face3D::FRONT, Orientation2D::UP, 0.0f);
    world.voxelInfo.setTextureData(1, Face3D::BACK, Orientation2D::UP, 0.0f);

    world.voxelInfo.setTextureData(2, Face3D::RIGHT, Orientation2D::UP, 1.0f);
    world.voxelInfo.setTextureData(2, Face3D::LEFT, Orientation2D::UP, 1.0f);
    world.voxelInfo.setTextureData(2, Face3D::TOP, Orientation2D::UP, 2.0f);
    world.voxelInfo.setTextureData(2, Face3D::BOTTOM, Orientation2D::UP, 1.0f);
    world.voxelInfo.setTextureData(2, Face3D::FRONT, Orientation2D::UP, 1.0f);
    world.voxelInfo.setTextureData(2, Face3D::BACK, Orientation2D::UP, 1.0f);

    //world.voxelInfo.setTransparent(1, false);
    //world.voxelInfo.setTransparent(2, false);
    
    std::cout << std::endl;

    int chunkCount = 1;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = 0; k < 8; k++) {
                std::cout << "\rGenerating chunks (" << chunkCount << "/" << 8*8*8 << ")" << std::flush;
                world.generateChunk(i-1,j,k);
                chunkCount++;
            }
        }
    }

    world.forceGlobalGeometryUpdate();

    voxelType = 1;
}

void TestScene::reInit() {

}

void TestScene::update(const float& dTime) {
    this->dTime = dTime;
    camera.update(dTime);
    handler.update();
    world.update();

    if (handler.isActionDown("Move Forward" )) camera.moveRelative(glm::vec3 (0.0f,  0.0f,  1.0f) * dTime * 4.0f);
    if (handler.isActionDown("Move Backward")) camera.moveRelative(glm::vec3( 0.0f,  0.0f, -1.0f) * dTime * 4.0f);
    if (handler.isActionDown("Move Left"    )) camera.moveRelative(glm::vec3(-1.0f,  0.0f,  0.0f) * dTime * 4.0f);
    if (handler.isActionDown("Move Right"   )) camera.moveRelative(glm::vec3( 1.0f,  0.0f,  0.0f) * dTime * 4.0f);
    if (handler.isActionDown("Move Up"      )) camera.move(glm::vec3( 0.0f,  1.0f,  0.0f) * dTime * 4.0f);
    if (handler.isActionDown("Move Down"    )) camera.move(glm::vec3( 0.0f,  -1.0f,  0.0f) * dTime * 4.0f);

    if (handler.isActionDown("Select Type 1")) voxelType = 1;
    if (handler.isActionDown("Select Type 2")) voxelType = 2;

    //place voxel testing
    if ((handler.isActionDown("Place Voxel") || handler.isActionDown("Delete Voxel")) && !placeVoxel) {
        placeVoxel = true;
        //perform a raycast
        glm::vec3 hitPos;
        glm::vec3 hitNormal;
        bool success;
        world.voxelMath.rayCast(hitPos, hitNormal, success, camera.getPosition(), camera.getAngleVector(), 10);
        if (success) {
            std::cout << "Hit voxel at: " << hitPos.x << ", " << hitPos.y << ", " << hitPos.z << std::endl;
            if (handler.isActionDown("Place Voxel")) {
                world.setVoxelType((int)(hitPos.x + hitNormal.x), (int)(hitPos.y + hitNormal.y), (int)(hitPos.z + hitNormal.z), voxelType, true);
            } else {
                world.setVoxelType((int)(hitPos.x), (int)(hitPos.y), (int)(hitPos.z), 0, true);
            }
        }
    } else if (!(handler.isActionDown("Place Voxel") || handler.isActionDown("Delete Voxel")) && placeVoxel) {
        placeVoxel = false;
    }

    //camera.moveRelative(camMovement * dTime);
}

void TestScene::draw() {
	mSkyboxTask->draw();

	gls::setShader(shaders);
    glUniformMatrix4fv(viewMatUni, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
    world.draw();
    
    glDisable(GL_DEPTH_TEST);
	mCursorShader.activate();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_DEPTH_TEST);
}

void TestScene::look() {
    int x, y; //mouse change
    SDL_GetRelativeMouseState(&x, &y);
    camera.rotate((float)x * dTime * sensitivity * 0.01f, (float)y * dTime * sensitivity * 0.01f);
    //camera.rotate((float)x, (float)y);
}

}
