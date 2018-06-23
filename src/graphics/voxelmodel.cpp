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

void VoxelModel::getFaceLighting(std::vector<unsigned short>& lighting, const FaceDirection& face, const std::array<unsigned short, 27>& surrounding_light, const unsigned& zero_weight) {

	/*
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
	*/

	auto average4 = [](unsigned short a, unsigned short b, unsigned short c, unsigned short d) {
		                unsigned short avg = (((a >> 12) & 0xF) +
		                                      ((b >> 12) & 0xF) +
		                                      ((c >> 12) & 0xF) +
		                                      ((d >> 12) & 0xF)) / 4;
		                avg = (avg << 4) | (((a >> 8) & 0xF) +
										    ((b >> 8) & 0xF) +
										    ((c >> 8) & 0xF) +
										    ((d >> 8) & 0xF)) / 4;
		                avg = (avg << 4) | (((a >> 4) & 0xF) +
										    ((b >> 4) & 0xF) +
										    ((c >> 4) & 0xF) +
										    ((d >> 4) & 0xF)) / 4;
		                avg = (avg << 4) | ((a & 0xF) +
										    (b & 0xF) +
										    (c & 0xF) +
										    (d & 0xF)) / 4;

		                return avg;
	                };
	
	auto& face_vec = mGeometry.at(static_cast<unsigned>(face));
	for (auto& i : face_vec) {
		//TODO: Find a more elegant solution that a big switch case with if statements
		//TODO: handle interpolation for verts that aren't on an edge

		/*
A brief explanation of the following code
it looks at the position of each vertice in the face mesh, checks which corner it is
closeest too, and then ges the average of the 4 adjacent light values with the
average4 lambda
		 */
		unsigned vx = (i >> 8) & 0xF;
		unsigned vy = (i >> 4) & 0xF;
		unsigned vz = i & 0xF;
		switch(face) {
		case FaceDirection::TOP:
			if (vx > 7) {
				if (vz > 7) {
					lighting.push_back(average4(surrounding_light[22],
					                            surrounding_light[23],
					                            surrounding_light[25],
					                            surrounding_light[26]));
				} else {
					lighting.push_back(average4(surrounding_light[22],
					                            surrounding_light[23],
					                            surrounding_light[19],
					                            surrounding_light[20]));
				}
			} else {
				if (vz > 7) {
					lighting.push_back(average4(surrounding_light[22],
					                            surrounding_light[21],
					                            surrounding_light[24],
					                            surrounding_light[25]));
				} else {
					lighting.push_back(average4(surrounding_light[22],
					                            surrounding_light[18],
					                            surrounding_light[19],
					                            surrounding_light[21]));
				}
			}
			break;

		case FaceDirection::BOTTOM:
			if (vx > 7) {
				if (vz > 7) {
					lighting.push_back(average4(surrounding_light[4],
					                            surrounding_light[5],
					                            surrounding_light[7],
					                            surrounding_light[8]));
				} else {
					lighting.push_back(average4(surrounding_light[4],
					                            surrounding_light[5],
					                            surrounding_light[1],
					                            surrounding_light[2]));
				}
			} else {
				if (vz > 7) {
					lighting.push_back(average4(surrounding_light[4],
					                            surrounding_light[3],
					                            surrounding_light[6],
					                            surrounding_light[7]));
				} else {
					lighting.push_back(average4(surrounding_light[4],
					                            surrounding_light[3],
					                            surrounding_light[0],
					                            surrounding_light[1]));
				}
			}
			break;
		case FaceDirection::NORTH:
			if (vx > 7) {
				if (vy > 7) {
					lighting.push_back(average4(surrounding_light[16],
					                            surrounding_light[17],
					                            surrounding_light[25],
					                            surrounding_light[26]));
				} else {
					lighting.push_back(average4(surrounding_light[16],
					                            surrounding_light[17],
					                            surrounding_light[7],
					                            surrounding_light[8]));
				}
			} else {
				if (vy > 7) {
					lighting.push_back(average4(surrounding_light[16],
					                            surrounding_light[15],
					                            surrounding_light[24],
					                            surrounding_light[25]));
				} else {
					lighting.push_back(average4(surrounding_light[16],
					                            surrounding_light[15],
					                            surrounding_light[6],
					                            surrounding_light[7]));
				}
			}
			break;
		case FaceDirection::SOUTH:
			if (vx > 7) {
				if (vy > 7) {
					lighting.push_back(average4(surrounding_light[10],
					                            surrounding_light[11],
					                            surrounding_light[19],
					                            surrounding_light[20]));
				} else {
					lighting.push_back(average4(surrounding_light[10],
					                            surrounding_light[11],
					                            surrounding_light[1],
					                            surrounding_light[2]));
				}
			} else {
				if (vy > 7) {
					lighting.push_back(average4(surrounding_light[10],
					                            surrounding_light[9],
					                            surrounding_light[18],
					                            surrounding_light[19]));
				} else {
					lighting.push_back(average4(surrounding_light[10],
					                            surrounding_light[9],
					                            surrounding_light[0],
					                            surrounding_light[1]));
				}
			}
			break;
		case FaceDirection::EAST:
			if (vz > 7) {
				if (vy > 7) {
					lighting.push_back(average4(surrounding_light[14],
					                            surrounding_light[17],
					                            surrounding_light[23],
					                            surrounding_light[26]));
				} else {
					lighting.push_back(average4(surrounding_light[14],
					                            surrounding_light[17],
					                            surrounding_light[5],
					                            surrounding_light[8]));
				}
			} else {
				if (vy > 7) {
					lighting.push_back(average4(surrounding_light[14],
					                            surrounding_light[11],
					                            surrounding_light[20],
					                            surrounding_light[23]));
				} else {
					lighting.push_back(average4(surrounding_light[14],
					                            surrounding_light[11],
					                            surrounding_light[2],
					                            surrounding_light[5]));
				}
			}
			break;
		case FaceDirection::WEST:
			if (vz > 7) {
				if (vy > 7) {
					lighting.push_back(average4(surrounding_light[12],
					                            surrounding_light[15],
					                            surrounding_light[21],
					                            surrounding_light[24]));
				} else {
					lighting.push_back(average4(surrounding_light[12],
					                            surrounding_light[15],
					                            surrounding_light[3],
					                            surrounding_light[6]));
				}
			} else {
				if (vy > 7) {
					lighting.push_back(average4(surrounding_light[12],
					                            surrounding_light[9],
					                            surrounding_light[18],
					                            surrounding_light[21]));
				} else {
					lighting.push_back(average4(surrounding_light[12],
					                            surrounding_light[9],
					                            surrounding_light[0],
					                            surrounding_light[3]));
				}
			}
			break;
		}
	}
}

}
