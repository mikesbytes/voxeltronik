#include "terrain/noise.h"
#include "sol.hpp"

namespace vtk { namespace noise {

Noise::Noise(const int64_t& seed) {
	mScale = 32.0;
	open_simplex_noise(seed, &mCtx);
}

Noise::~Noise() {
	open_simplex_noise_free(mCtx);
}

double Noise::get2D(const double& x, const double& y) {
	return open_simplex_noise2(mCtx,
			x / mScale,
			y / mScale);
}

double Noise::get3D(const double& x, const double& y, const double& z) {
	return open_simplex_noise3(mCtx,
			x / mScale,
			y / mScale,
			z / mScale);

}

void Noise::setScale(const double& scale) {
	mScale = scale;
}

void Noise::registerScriptInterface(::sol::state &lua) {
	lua.new_usertype<Noise>("Noise",
	                        sol::constructors<Noise(const int64_t&)>(),
	                        "get_2d", &Noise::get2D,
	                        "get_3d", &Noise::get3D,
	                        "set_scale", &Noise::setScale,
	                        sol::base_classes, sol::bases<NoiseModule>());
}

}}
	
