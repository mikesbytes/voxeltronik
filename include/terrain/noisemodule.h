/*
 * Module on which noise generation objects sit
 */

#pragma once

namespace sol {
class state;
}

namespace vtk { namespace noise {

class NoiseModule {
public:
	virtual double get2D(const double& x, const double& y){ return 0.0; }
	virtual double get3D(const double& x, const double& y, const double& z) { return 0.0; }

	static void registerScriptInterface(::sol::state& lua);
};

  }} //vtk::noise
