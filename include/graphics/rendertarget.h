/*
 * Rendertarget like an FBO or window
 */

#pragma once

namespace vtk {

class RenderTarget {
public:
	virtual void activate() = 0;
	double getAspect();
	int width();
	int height();
protected:
	int mWidth;
	int mHeight;
};

}
