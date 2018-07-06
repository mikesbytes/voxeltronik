/*
 * Rendertarget like an FBO or window
 */

#pragma once

namespace sol {
class state;
}

namespace vtk {

class RenderTarget {
public:
	virtual void activate() {};
	double getAspect();
	int width();
	int height();

	static void registerScriptInterface(::sol::state& lua);
protected:
	int mWidth;
	int mHeight;
};

}
