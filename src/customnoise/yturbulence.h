/*
 * =====================================================================================
 *
 *       Filename:  yturbulence.h
 *
 *    Description:  Y axis turbulence using perlin,
 *                  Basically just normal turbulence, but only on Y.
 *
 *        Version:  1.0
 *        Created:  05/10/2014 06:11:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include <noise/noise.h>


//TODO: Add support for more than 2 stops

namespace vtk {

class YTurbulence : public noise::module::Module {
public:
    YTurbulence();

    virtual int GetSourceModuleCount() const;
    virtual double GetValue(double x, double y, double z) const;

    noise::module::Perlin yNoise;
    double power;
protected:

};


}
