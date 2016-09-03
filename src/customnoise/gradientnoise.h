/*
 * =====================================================================================
 *
 *       Filename:  gradientnoise.h
 *
 *    Description:  Produces a vertical gradient
 *
 *        Version:  1.0
 *        Created:  05/10/2014 03:42:45 PM
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

class GradientNoise : public noise::module::Module {
public:
    GradientNoise();

    virtual int GetSourceModuleCount() const;
    virtual double GetValue(double x, double y, double z) const;

    void setStop(const int& index, const double& value);

protected:
    double stop0;
    double stop1;
};


}
