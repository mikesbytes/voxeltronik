#include "terrain/noise.h"

namespace vtk {

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

}
	
