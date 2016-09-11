/*
 * =====================================================================================
 *
 *       Filename:  drawable.h
 *
 *    Description:  Drawable object
 *
 *        Version:  1.0
 *        Created:  05/20/2014 11:02:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "../camera.h"

#include <memory>

namespace vtk {

//Forward declarations
class Shader;

///\brief Abstract base class that can 
///   be drawn to a camera

class Drawable {
public:
    ///\brief Destructor
    virtual ~Drawable();

    ///\brief Draw something from the viewpoint of a camera
    ///\param camera Camera to draw with
    virtual void draw(Camera& camera);
protected:
    std::shared_ptr<Shader> linkedShader;
};

}
