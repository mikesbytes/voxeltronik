#include "graphics/rendertask.h"
#include "graphics/camera.h"
#include "graphics/drawable.h"
#include "graphics/rendertarget.h"

namespace vtk {

RenderTask::RenderTask(Drawable& drawable, Camera& camera, RenderTarget& renderTarget) :
	mDrawable(drawable),
	mCamera(camera),
	mRenderTarget(renderTarget)
{

}

void RenderTask::draw() {
	mRenderTarget.activate();
	mDrawable.draw(*this);
}

}
