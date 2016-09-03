/*
 * =====================================================================================
 *
 *       Filename:  yturbulence.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/10/2014 06:17:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "yturbulence.h"

namespace vtk {
YTurbulence::YTurbulence() : Module(GetSourceModuleCount()){

}

int YTurbulence::GetSourceModuleCount() const {
    return 1;
}

double YTurbulence::GetValue(double x, double y, double z) const {
    double xOff, yOff, zOff;
    xOff = x + (12414.0 / 65536.0);
    yOff = y + (65124.0 / 65536.0);
    zOff = z + (31337.0 / 65536.0);

    double distortion = y + (yNoise.GetValue(xOff, yOff, zOff) * power);

    return m_pSourceModule[0]->GetValue(x, distortion, z);
}

}
