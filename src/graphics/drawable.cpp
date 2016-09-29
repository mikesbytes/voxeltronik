#include "graphics/drawable.h"

namespace vtk {


void Drawable::setShader(std::shared_ptr<Shader> shader) {
	mLinkedShader = shader;
}

std::shared_ptr<Shader> Drawable::getShader() {
	return mLinkedShader;
}

}
