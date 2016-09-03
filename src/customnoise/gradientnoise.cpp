/*
 * =====================================================================================
 *
 *       Filename:  gradientnoise.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/10/2014 03:47:54 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "gradientnoise.h"

namespace vtk {

GradientNoise::GradientNoise() : Module(GetSourceModuleCount()){

}

int GradientNoise::GetSourceModuleCount() const {
    return 0;
}

double GradientNoise::GetValue(double x, double y, double z) const {
    //We don't need x and z, so we suppress the warnings about it
    (void)x;
    (void)z;

    if (y <= stop0) return -1.0;
    if (y >= stop1) return 1.0;

    return ((y - stop0) / (stop1 - stop0)) * 2 - 1;
}

void GradientNoise::setStop(const int& index, const double& value) {
    if (index == 0) stop0 = value;
    else if (index == 1) stop1 = value;
}

}

