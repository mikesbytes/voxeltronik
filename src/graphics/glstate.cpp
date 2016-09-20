#include <vector>
#include <utility>
#include <iostream>

#include "graphics/glstate.h"
#include "graphics/shader.h"

namespace vtk::gls {
	
	static GLuint activeShader(0);
	static bool trackingEnabled(true);
	static std::vector<std::pair<GLenum, bool> > flags;

	int setShader(const GLuint& shaderID) {
		if (activeShader == shaderID) return 0;
		glUseProgram(shaderID);
		activeShader = shaderID;
	}

	void setTracking(const bool& flag) {
		trackingEnabled = flag;
	}

	void setFlag(const GLenum& flag, const bool& newState) {
		if (trackingEnabled) {
			GLboolean state;
			glGetBooleanv(flag, &state);
			if ((bool)state != newState) {
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
		for(auto& i : flags) {
			if(i.second) glDisable(i.first);
			else glEnable(i.first);
		}
		flags.clear();
	}
}
