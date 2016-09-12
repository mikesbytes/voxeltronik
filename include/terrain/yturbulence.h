/*
 * Y Axis turbulence
 */

#pragma once
#include <memory>

#include "terrain/noisemodule.h"

namespace vtk::noise {

class YTurbulence : public NoiseModule {
public:
	YTurbulence(std::shared_ptr<NoiseModule> input, std::shared_ptr<NoiseModule> modifier);
	YTurbulence(std::shared_ptr<NoiseModule> input, std::shared_ptr<NoiseModule> modifier, const double& multiplier);
	double get3D(const double&x, const double&y, const double&z);

protected:
	std::shared_ptr<NoiseModule> mInput;
	std::shared_ptr<NoiseModule> mModifier;
	double mMultiplier;
};
}
