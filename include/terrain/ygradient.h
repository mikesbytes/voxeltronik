/*
 * Prouces a Y axis gradient with -1.0 at start and 1.0 at finish
 */

#pragma once

#include "terrain/noisemodule.h"

namespace vtk {

class YGradient : public NoiseModule {
public:
	YGradient();
	YGradient(const double& start, const double& end);

	void setStart(const double& start);
	void setEnd(const double& end);

	double get3D(const double& x, const double& y, const double& z);

protected:
	double mStart;
	double mEnd;
};

}
