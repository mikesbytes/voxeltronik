/*
 * =====================================================================================
 *
 *       Filename:  shader.h
 *
 *    Description:  Shader that shades
 *
 *        Version:  1.0
 *        Created:  05/12/2014 10:54:44 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include <string>

namespace vtk {

class Shader {
public:
    bool activate(); //Must be called to use shader

    //Load shader, type 0 = Vert, 1 = Frag, 2 = Geometry
    bool loadShaderFromFile(const std::string& fileName, const unsigned& type);
    


protected:

};

}
