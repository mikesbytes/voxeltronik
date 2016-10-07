#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include "graphics/camera.h"
#include "graphics/drawable.h"
#include "graphics/rendertask.h"

namespace vtk {

	class Skybox : public Drawable {
	public:
		Skybox();

		void draw(RenderTask& task);
	protected:
		GLuint mVbo;
		GLuint mVao;
		GLuint mShader;
		GLuint mViewMat;
		GLuint mProjMat;
	};
}
