/*
 * Module on which noise generation objects sit
 */

#pragma once

namespace vtk { namespace noise {

class NoiseModule {
public:
	virtual double get2D(const double& x, const double& y){ return 0.0; }
	virtual double get3D(const double& x, const double& y, const double& z) { return 0.0; }
};

  }} //vtk::noise
