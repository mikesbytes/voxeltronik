#include <iostream>

#include "terrain/yturbulence.h"

namespace vtk { namespace noise {

YTurbulence::YTurbulence(std::shared_ptr<NoiseModule> input, std::shared_ptr<NoiseModule> modifier) {
	mInput = input;
	mModifier = modifier;
}

YTurbulence::YTurbulence(std::shared_ptr<NoiseModule> input, std::shared_ptr<NoiseModule> modifier, const double& multiplier) {
	mInput = input;
	mModifier = modifier;
	mMultiplier = multiplier;
}

double YTurbulence::get3D(const double&x, const double&y, const double&z) {
	double mod = mModifier->get3D(x,y,z) * mMultiplier;
	return mInput->get3D(x, y + mod, z);
}

  }}
