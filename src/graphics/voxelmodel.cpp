#include <iostream>
#include "graphics/voxelmodel.h"

namespace vtk {

VoxelModel::VoxelModel() {
	//placeholder, just build a cube
	std::vector<unsigned> top = {
		0, 15,  0, 15, 0,
		15, 15, 15, 15, 0,
		0, 0,   0, 15, 15,
		15, 0,  15, 15, 15,
		0, 0,   0, 15, 15,
		15, 15, 15, 15, 0
	};
	packDataIntoFace(top, 0);

	std::vector<unsigned> bottom = {
		0, 15, 	0, 0, 15,
		15, 15,	15, 0, 15,
		0, 0,  	0, 0, 0,
		15, 0, 	15, 0, 0,
		0, 0,  	0, 0, 0,
		15, 15,	15, 0, 15
	};
	packDataIntoFace(bottom, 1);

	std::vector<unsigned> north = {
		0, 15, 	0, 15, 15,
		15, 15,	15, 15, 15,
		0, 0,  	0, 0, 15,
		15, 0, 	15, 0, 15,
		0, 0,  	0, 0, 15,
		15, 15,	15, 15, 15
	};
	packDataIntoFace(north, 2);

	std::vector<unsigned> south = {
		0, 15, 	15, 15, 0,
		15, 15,	0, 15, 0,
		0, 0,  	15, 0, 0,
		15, 0, 	0, 0, 0,
		0, 0,  	15, 0, 0,
		15, 15,	0, 15, 0
	};
	packDataIntoFace(south, 3);

	std::vector<unsigned> east = {
		0, 15, 	15, 15, 15,
		15, 15,	15, 15, 0,
		0, 0,  	15, 0, 15,
		15, 0, 	15, 0, 0,
		0, 0,  	15, 0, 15,
		15, 15,	15, 15, 0
	};
	packDataIntoFace(east, 4);

	std::vector<unsigned> west = {
		0, 15, 	0, 15, 0,
		15, 15,	0, 15, 15,
		0, 0,  	0, 0, 0,
		15, 0, 	0, 0, 15,
		0, 0,  	0, 0, 0,
		15, 15,	0, 15, 15
	};
	packDataIntoFace(west, 5);

}

VoxelModel::~VoxelModel() {

}

bool VoxelModel::packDataIntoFace(const std::vector<unsigned>& data, const int& face) {
	for (int i = 0; i < data.size(); i += 5) {
		unsigned vertex = 0;
		vertex = (vertex     ) | data[i    ];
		vertex = (vertex << 4) | data[i + 1];
		vertex = (vertex << 4) | data[i + 2];
		vertex = (vertex << 4) | data[i + 3];
		vertex = (vertex << 4) | data[i + 4];
		//std::cout << vertex << " | ";

		mGeometry[face].push_back(vertex);
	}
	return true;
}



bool VoxelModel::uploadFaceToMesh(std::vector<unsigned>& mesh, const int& face) {
	auto& faceModel = mGeometry[face];
	if (faceModel.empty()) return false;

	for (auto& i : faceModel) {
		mesh.push_back(i);
	}
	
	return true;
}

std::vector<unsigned>& VoxelModel::getFaceMesh(const int& face) {
	return mGeometry[face];
}
}
