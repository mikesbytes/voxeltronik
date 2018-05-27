#pragma once

#include <vector>
#include <array>

#include "voxelmath.h"

namespace vtk {

class VoxelModel {
public:
	VoxelModel();
	virtual ~VoxelModel();

	// accepts values between 0 and 15 to be packed into the geometry of face
	bool packDataIntoFace(const std::vector<unsigned>& data, const int& face);
	bool uploadFaceToMesh(std::vector<unsigned>& mesh, const int& face);
	std::vector<unsigned>& getFaceMesh(const int& face);

	/* surrounding light is an array containing the light data around the block in the following configuration
	
	   where X stands for none and any numbers stand for array indexes
	  
	         y = 1  y = 0  y = -1
	      x -1 0 1  
	    z -1 0 X 1  X 5 X  9  X 10
	       0 X 2 X  6 X 7  X 11 X
	       1 3 X 4	X 8 X  12 X 13
	        
	zero_weight controls how "smoothed" the lighting is. Read the function body for more info on how this works
	*/
	void  getFaceLighting(std::vector<unsigned>& lighting, const FaceDirection& face, const std::array<unsigned, 14>& surrounding_light, const unsigned& zero_weight);
protected:
	// top bottom north south east west, special
	std::array<std::vector<unsigned>, 7> mGeometry;
};

} //namespace vtk
