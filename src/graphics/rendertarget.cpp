#include "graphics/rendertarget.h"

namespace vtk {

double RenderTarget::getAspect() {
	return (double)mWidth / (double)mHeight;
}

int RenderTarget::width() {
	return mWidth;
}

int RenderTarget::height() {
	return mHeight;
}

}
