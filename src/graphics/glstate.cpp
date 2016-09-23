#include <vector>
#include <utility>
#include <iostream>

#include "graphics/glstate.h"
#include "graphics/shader.h"

namespace vtk::gls {
	
	static GLuint activeShader(0);
	static bool trackingEnabled(false);
	static std::vector<std::pair<GLenum, bool> > flags;

	int setShader(const GLuint& shaderID) {
		//check that the shader we want to set isn't already active
		if (activeShader == shaderID){ return 0; }
		//set the shader
		glUseProgram(shaderID);
		activeShader = shaderID;
		return 1;
	}

	void setTracking(const bool& flag) {
		trackingEnabled = flag;
	}

	void setFlag(const GLenum& flag, const bool& newState) {
		if (trackingEnabled) {
			//check if flag already === newState
			GLboolean state;
			glGetBooleanv(flag, &state);
			if ((bool)state != newState) {
				//set flag and push the new status into the flags list
				if (newState) glEnable(flag);
				else glDisable(flag);

				flags.push_back(std::make_pair(flag,newState));
			}
		} else {
			if (newState) glEnable(flag);
			else glDisable(flag);
		}
	}
	
	void restoreState() {
		if (flags.empty()) return;

		//iterate through flags and reset them
		for(auto& i : flags) {
			if(i.second) glDisable(i.first);
			else glEnable(i.first);
		}
		flags.clear();
	}
}
