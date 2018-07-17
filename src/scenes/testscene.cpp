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

#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

#include <iostream>

namespace vtk {

void TestScene::init() {

	Tileset tiles;
	tiles.buildTexture();
	tiles.updateTextureAt(0, "res/stone.png");
	tiles.updateTextureAt(1, "res/dirt.png");
	tiles.updateTextureAt(2, "res/grass.png");
	tiles.updateTextureAt(3, "res/test.png");
	tiles.updateTextureAt(4, "res/test-blue.png");

	//shaders
	shaders = LoadShaders("res/shaders/voxelvert.vert.glsl", "res/shaders/voxelfrag.frag.glsl");

	glUseProgram(shaders);

	mCamLastLoadPosition = glm::vec3(0.0f, 64.0f, 0.0f);
	camera.setPosition(glm::vec3(0.0f, 64.0f, 0.0f));
	camera.setAspectRatio(linkedGame->window.getAspect());

	viewMatUni = glGetUniformLocation(shaders, "view");
	projMatUni = glGetUniformLocation(shaders, "proj");
	modelMatUni = glGetUniformLocation(shaders, "model");
	world.modelMatUni = modelMatUni;
	mSkyboxTask = new RenderTask(mSkybox, camera, linkedGame->window);

	glUniformMatrix4fv(viewMatUni, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
	glUniformMatrix4fv(projMatUni, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));

	//nanovg
	vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
	nvgCreateFont(vg, "mono", "res/fonts/DejaVuSansMono.ttf");

	//sdl mouse settings
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
	handler.setAction("Select Type 3", conf->getValue<std::string>("controls.bindings.typesel.type3",     "3"        ));
	handler.setAction("Select Type 4", conf->getValue<std::string>("controls.bindings.typesel.type4",     "4"        ));
	handler.setAction("Toggle Noclip", conf->getValue<std::string>("controls.bindings.noclip", "V"));

	//set signals for handler
	handler.getEventSignal(SDL_QUIT       ).connect<Game     , &Game::stop     >(linkedGame);
	handler.getEventSignal(SDL_MOUSEMOTION).connect<TestScene, &TestScene::look>(this);


	world.voxelInfo.setAllTextureIndexes(1, 0);
	world.voxelInfo.setAllTextureIndexes(2, 1);
	world.voxelInfo.setTextureIndex(2, FaceDirection::TOP, 2);
	world.voxelInfo.setAllTextureIndexes(3, 3);
	world.voxelInfo.setAllTextureIndexes(4, 4);

	world.voxelInfo.setTransparent(0, true);
	world.voxelInfo.setTransparent(1, false);
	world.voxelInfo.setTransparent(2, false);
	world.voxelInfo.setTransparent(3, false);
	world.voxelInfo.setTransparent(4, false);


	world.voxelInfo.setEmission(0, 0);
	world.voxelInfo.setEmission(1, 0);
	world.voxelInfo.setEmission(2, 0);
	world.voxelInfo.setEmission(3, 0xF4B0);
	world.voxelInfo.setEmission(4, 0x00F0);

	std::cout << std::endl;

	world.queueChunkLoadsAroundPoint(glm::vec3(0.0,0.0,0.0), 20);

	//world.forceGlobalGeometryUpdate();

	voxelType = 1;
	mNoclip = true;
	mNoclipDebounce = false;
}

void TestScene::reInit() {

}

void TestScene::update(const float& dTime) {
	this->dTime = dTime;
	camera.update(dTime);
	handler.update();
	world.update();

	mFPS = 1.0f/dTime;

	if (handler.isActionDown("Move Forward" )) camera.moveRelative(glm::vec3 (0.0f,  0.0f,  1.0f) * dTime * 16.0f);
	if (handler.isActionDown("Move Backward")) camera.moveRelative(glm::vec3( 0.0f,  0.0f, -1.0f) * dTime * 16.0f);
	if (handler.isActionDown("Move Left"    )) camera.moveRelative(glm::vec3(-1.0f,  0.0f,  0.0f) * dTime * 16.0f);
	if (handler.isActionDown("Move Right"   )) camera.moveRelative(glm::vec3( 1.0f,  0.0f,  0.0f) * dTime * 16.0f);
	if (handler.isActionDown("Move Up"      ) && mNoclip) camera.move(glm::vec3( 0.0f,  1.0f,  0.0f) * dTime * 8.0f);
	if (handler.isActionDown("Move Down"    ) && mNoclip) camera.move(glm::vec3( 0.0f,  -1.0f,  0.0f) * dTime * 8.0f);

	if (handler.isActionDown("Select Type 1")) voxelType = 1;
	if (handler.isActionDown("Select Type 2")) voxelType = 2;
	if (handler.isActionDown("Select Type 3")) voxelType = 3;
	if (handler.isActionDown("Select Type 4")) voxelType = 4;
	if (!mNoclipDebounce && handler.isActionDown("Toggle Noclip")) {
		mNoclipDebounce = true;
		mNoclip = !mNoclip;
		std::cout << mNoclip << "\n" << std::flush;
	} else {
		//mNoclipDebounce = false;
	}

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
			std::cout << "Height @ " << world.getHeight(glm::ivec2(hitPos.x + hitNormal.x, hitPos.z + hitNormal.z)) << std::endl;
			if (handler.isActionDown("Place Voxel")) {
				world.placeVoxel(hitPos + hitNormal, voxelType);
				//world.setVoxelType((int)(hitPos.x + hitNormal.x), (int)(hitPos.y + hitNormal.y), (int)(hitPos.z + hitNormal.z), voxelType, true);
			} else {
				world.breakVoxel(hitPos);
				//world.setVoxelType((int)(hitPos.x), (int)(hitPos.y), (int)(hitPos.z), 0, true);
			}
		}
	} else if (!(handler.isActionDown("Place Voxel") || handler.isActionDown("Delete Voxel")) && placeVoxel) {
		placeVoxel = false;
	}

	float distance = glm::distance(camera.getPosition(), mCamLastLoadPosition);
	if (distance >= 16.0f) {
		mCamLastLoadPosition = camera.getPosition();
		//world.queueChunkLoadsAroundPoint(camera.getPosition(), 16);
	}

	//player "physics"
	if (!mNoclip) {
		mVelocity = glm::vec3(0.0f, mVelocity.y + (-9.8f * dTime), 0.0f);
		// check ground collision
		glm::ivec3 cameraIPos = camera.getPosition();
		cameraIPos.y = cameraIPos.y - 2;
		if (world.isVoxelSolid(cameraIPos.x, cameraIPos.y, cameraIPos.z)) {
			std::cout << cameraIPos.y << "\n" << std::flush;
			auto newCameraPos = camera.getPosition();
			newCameraPos.y = (float)cameraIPos.y + 2.8f;
			camera.setPosition(newCameraPos);
			mVelocity.y = 0.0f;
		}
		camera.move(mVelocity * dTime);
	}
}

void TestScene::draw() {
	mSkyboxTask->draw();

	//draw world TODO: move this to rendertasks
	glUseProgram(shaders);
	glUniformMatrix4fv(viewMatUni, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
	glUniformMatrix4fv(projMatUni, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
	//world.draw();

	glDisable(GL_DEPTH_TEST);

	int nvgWidth = 1920;
	int nvgHeight = 1080;
	
	nvgBeginFrame(vg, nvgWidth, nvgHeight, 1);

	nvgFontSize(vg, 14.0f);
	nvgFontFace(vg, "mono");
	nvgFillColor(vg, nvgRGBA(0, 0, 0, 190));

	std::string fpsString = std::to_string(mFPS);
	nvgText(vg, 5, 19, fpsString.data(), NULL);

	nvgBeginPath(vg);
	nvgRect(vg, ((float)nvgWidth / 2.0f) - 15.0f, ((float)nvgHeight / 2.0f) - 15.0f, 30.0f, 30.0f);
	nvgFillColor(vg, nvgRGBA(153, 0, 204, 80));
	nvgFill(vg);

	nvgEndFrame(vg);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void TestScene::look() {
	int x, y; //mouse change
	SDL_GetRelativeMouseState(&x, &y);
	camera.rotate((float)x * sensitivity * 0.001f, (float)y * sensitivity * 0.001f);
	//camera.rotate((float)x, (float)y);
}

}
