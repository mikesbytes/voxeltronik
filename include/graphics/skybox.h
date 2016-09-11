#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include "camera.h"

namespace vtk {

	class Skybox {
	public:
		Skybox();

		void draw(Camera& camera, glm::mat4 proj);
	protected:
		GLuint mVbo;
		GLuint mVao;
		GLuint mShader;
		GLuint mViewMat;
		GLuint mProjMat;
	};
}
