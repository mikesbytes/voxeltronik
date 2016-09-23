/*
 * Functions for dealing with OGL's PITA state management
 */

#pragma once

#include <memory>

#include <GL/glew.h>
#include <GL/gl.h>

// forward declarations
namespace vtk {
	class Shader;
}

namespace vtk::gls {

	// set active shader
	int setShader(const GLuint& shaderID);
	// enable tracking of state changes
	void setTracking(const bool& flag);
	void setFlag(const GLenum& flag, const bool& newState);
	void restoreState();

}
