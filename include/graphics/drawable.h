/*
 * Drawable class
 */


#include <memory>

namespace vtk {

//Forward declarations
class Shader;
class RenderTask;


class Drawable {
public:
    virtual ~Drawable();

	//Draw (called from rednertask)
    virtual void draw(RenderTask& task) = 0;

	void setShader(std::shared_ptr<Shader>);
	std::shared_ptr<Shader> getShader();

protected:
    std::shared_ptr<Shader> mLinkedShader;
};

}
