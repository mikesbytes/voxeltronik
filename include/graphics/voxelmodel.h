#pragma once

#include <vector>
#include <array>

namespace vtk {

class VoxelModel {
public:
	VoxelModel();
	virtual ~VoxelModel();

	// accepts values between 0 and 15 to be packed into the geometry of face
	bool packDataIntoFace(const std::vector<unsigned>& data, const int& face);
	bool uploadFaceToMesh(std::vector<unsigned>& mesh, const int& face);
	std::vector<unsigned>& getFaceMesh(const int& face);
protected:
	// top bottom north south east west
	std::array<std::vector<unsigned>, 7> mGeometry;
};

} //namespace vtk
