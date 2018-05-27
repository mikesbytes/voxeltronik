#include <iostream>
#include <glm/glm.hpp>
#include <algorithm>

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

void VoxelModel::getFaceLighting(std::vector<unsigned>& lighting, const FaceDirection& face, const std::array<unsigned, 14>& surrounding_light, const unsigned& zero_weight) {

	auto blend = [](unsigned a, unsigned b, unsigned weight) {
		unsigned aw = 7 - weight;
		unsigned bw = weight;
		unsigned newLight = 0;
		newLight = std::min(((((a >> 24) & 0xFF) * aw) / 8u) +
		                    ((((b >> 24) & 0xFF) * bw) / 8u), 255u); //feels like lisp
		newLight = (newLight << 8) |
			std::min(((((a >> 16) & 0xFF) * aw) / 7u) +
			         ((((b >> 16) & 0xFF) * bw) / 7u), 255u);
		newLight = (newLight << 8) |
			std::min(((((a >> 8) & 0xFF) * aw) / 7u) +
			         ((((b >> 8) & 0xFF) * bw) / 7u), 255u);
		newLight = (newLight << 8) |
			std::min(((((a) & 0xFF) * aw) / 7u) +
			         ((((b) & 0xFF) * bw) / 7u), 255u);
		return newLight;
		    };
	
	auto& face_vec = mGeometry.at(static_cast<unsigned>(face));
	for (auto& i : face_vec) {
		unsigned vx = (i >> 8) & 0xF;
		unsigned vy = (i >> 4) & 0xF;
		unsigned vz = i & 0xF;
		switch(face) {
		case FaceDirection::TOP:

			//std::cout << vx << ", " << vz << "\n";
			if (vx > 7) {
				if (vz > 7) {
					std::cout << "hit\n";
					lighting.push_back(surrounding_light[4]); //4
				} else {
					lighting.push_back(surrounding_light[1]); //1
				}
			} else {
				if (vz > 7) {
					lighting.push_back(surrounding_light[3]); //3
				} else {
					lighting.push_back(surrounding_light[0]); //0
				}
			}
			/*
			lighting.push_back(blend(surrounding_light[2],
			                         surrounding_light[4],
			                         (vx + vy - 14u) / 2u));
			*/
			break;

		case FaceDirection::BOTTOM:
			lighting.push_back(surrounding_light[11]);
			break;
		case FaceDirection::NORTH:
			lighting.push_back(surrounding_light[8]);
			break;
		case FaceDirection::SOUTH:
			lighting.push_back(surrounding_light[5]);
			break;
		case FaceDirection::EAST:
			lighting.push_back(surrounding_light[7]);
			break;
		case FaceDirection::WEST:
			lighting.push_back(surrounding_light[6]);
			break;
		default:
			lighting.push_back(0xFFFFFFFF);
			break;
		}
	}
}

}
