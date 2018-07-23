/*
 * Simple noise module utilizing smcameron's c port of OpenSimplex
 */

#pragma once

#include "terrain/open_simplex_noise.h"
#include "terrain/noisemodule.h"

namespace sol {
class state;
}

namespace vtk { namespace noise {

class Noise : public NoiseModule {
public:
	Noise(const int64_t& seed);
	~Noise();
	
	double get2D(const double& x, const double& y);
	double get3D(const double& x, const double& y, const double& z);
	void setScale(const double& scale);

	static void registerScriptInterface(::sol::state& lua);
protected:
	double mScale;
	struct osn_context* mCtx;
};

  }} //vtk::noise
