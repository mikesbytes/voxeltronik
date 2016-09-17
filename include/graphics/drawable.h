/*
 * Drawable class
 */

#include "../camera.h"

#include <memory>

namespace vtk {

//Forward declarations
class Shader;
class RenderTask;


class Drawable {
public:
    ///\brief Destructor
    virtual ~Drawable();

	//Draw (called from rednertask)
    virtual void draw(RenderTask& task);
protected:
    std::shared_ptr<Shader> linkedShader;
};

}
