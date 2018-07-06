#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include "graphics/camera.h"
#include "graphics/drawable.h"
#include "graphics/rendertask.h"

namespace sol {
class state;
}

namespace vtk {
class Skybox : public Drawable {
public:
	Skybox();

	void draw(RenderTask& task);

	static void registerScriptInterface(::sol::state& lua);
protected:
	GLuint mVbo;
	GLuint mVao;
	GLuint mShader;
	GLuint mViewMat;
	GLuint mProjMat;
};
}
