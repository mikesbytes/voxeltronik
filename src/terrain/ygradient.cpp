#include "terrain/ygradient.h"

#include "sol.hpp"

namespace vtk { namespace noise {

YGradient::YGradient() {
	mStart = 0.0;
	mEnd = 0.0;
}

YGradient::YGradient(const double& start, const double& end) {
	mStart = start;
	mEnd = end;
}

void YGradient::setStart(const double& start) {
	mStart = start;
}

void YGradient::setEnd(const double& end) {
	mEnd = end;
}

double YGradient::get3D(const double& x, const double& y, const double& z) {
	double factor = (y - mStart) / (mEnd - mStart);
	if (factor >= 1.0) return 1.0;
	else if (factor <= 0.0) return -1.0;
	return (factor * 2.0) - 1.0;
}

void YGradient::registerScriptInterface(::sol::state &lua) {
	lua.new_usertype<YGradient>("YGradient",
	                            sol::constructors<YGradient(), YGradient(const double&, const double&)>(),
	                            sol::base_classes, sol::bases<NoiseModule>());
}
}}
