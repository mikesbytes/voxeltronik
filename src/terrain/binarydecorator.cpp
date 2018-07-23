#include "terrain/binarydecorator.h"
#include "terrain/noisemodule.h"

#include "sol.hpp"

namespace vtk { namespace noise {

BinaryDecorator::BinaryDecorator(NoiseModule& noise, const double& threshold, const unsigned& low, const unsigned& high) :
	mNoise(noise),
	mThreshold(threshold),
	mLow(low),
	mHigh(high)
{

}

unsigned BinaryDecorator::get3D(const glm::ivec3& pos) {
	double nVal = mNoise.get3D((double)pos.x, (double)pos.y, (double)pos.z);
	if (nVal >= mThreshold) return mHigh;
	return mLow;
}

void BinaryDecorator::registerScriptInterface(::sol::state &lua) {
	lua.new_usertype<BinaryDecorator>("BinaryDecorator",
	                                  sol::constructors<BinaryDecorator(NoiseModule&, const double&, const unsigned&, const unsigned&)>(),
	                                  sol::base_classes, sol::bases<Decorator>());
}

}}
