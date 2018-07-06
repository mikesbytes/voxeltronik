/*
 * Drawable class
 */

#pragma once
#include <memory>

namespace sol {
class state;
}

namespace vtk {

//Forward declarations
class Shader;
class RenderTask;


class Drawable {
public:
    virtual ~Drawable();

	//Draw (called from rednertask)
	virtual void draw(RenderTask& task) {}

	void setShader(std::shared_ptr<Shader>);
	std::shared_ptr<Shader> getShader();

	static void registerScriptInterface(::sol::state& lua);

protected:
    std::shared_ptr<Shader> mLinkedShader;
};

}
