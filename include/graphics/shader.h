/*
 * Loads and manages a shader
 */

#pragma once

#include <string>

namespace vtk {

class Shader {
public:
    bool activate(); //Must be called to use shader

    //Load shader, type 0 = Vert, 1 = Frag, 2 = Geometry
    bool loadShaderFiles(const std::string& vertShader,
	                     const std::string& fragShader,
					     const std::string& geomShader = "");

    


protected:

};

}
