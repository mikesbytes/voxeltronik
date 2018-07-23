/* when noise outputs greater than or equal to the threshold
the "high" type is output, when noise output
is lower than the threshold the "low" type is output*/

#pragma once

#include "terrain/decorator.h"

namespace sol {
class state;
}

namespace vtk { namespace noise {

class NoiseModule;

class BinaryDecorator : public Decorator {
public:
	BinaryDecorator(NoiseModule& noise, const double& threshold, const unsigned& low, const unsigned& high);
	unsigned get3D(const glm::ivec3& pos);

	static void registerScriptInterface(::sol::state& lua);
protected:
	NoiseModule& mNoise;
	double mThreshold;
	unsigned mLow;
	unsigned mHigh;
};

}}
