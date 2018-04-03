
/*
 * =====================================================================================
 *
 *       Filename:  voxelinfo.cpp
 *
 *    Description:  Voxelinfo source file
 *
 *        Version:  1.0
 *        Created:  04/06/2014 01:30:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "voxelinfo.h"
#include "world.h"

namespace vtk {

void VoxelInfo::pushTexCoordFromWorldCoords(std::vector<float>& data, const glm::ivec3& pos, const Face3D& face, const Corner2D& corner) {
    pushTexCoordFromVoxelID(data, linkedWorld->getVoxelType(pos), face, corner);
}

void VoxelInfo::pushTexCoordFromVoxelID(std::vector<float>& data, const unsigned& id, const Face3D& face, const Corner2D& corner) {
    auto coord = getTexCoordFromID(id, face, corner);
    data.push_back(coord.first);
    data.push_back(coord.second);
    data.push_back(getTexIndexFromID(id, face));
}

float VoxelInfo::getTexIndexFromID(const unsigned& id, const Face3D& face) {
    if (voxelDataMap.find(id) == voxelDataMap.end()) return 0.0f;
    return voxelDataMap[id].faceTextureIndices[face];
}

std::pair<float, float> VoxelInfo::getTexCoordFromID(const unsigned& id, const Face3D& face, const Corner2D& corner) {
    if (voxelDataMap.find(id) == voxelDataMap.end()) return std::make_pair(0.0f, 0.0f);
    Orientation2D orient = voxelDataMap[id].faceOrientation[face];

    switch (orient) {
        case Orientation2D::UP:
            switch (corner) {
                case Corner2D::TOPLEFT:
                    return std::make_pair(0.0f, 1.0f);
                    break;
                case Corner2D::TOPRIGHT:
                    return std::make_pair(1.0f, 1.0f);
                    break;
                case Corner2D::BOTTOMLEFT:
                    return std::make_pair(0.0f, 0.0f);
                    break;
                case Corner2D::BOTTOMRIGHT:
                    return std::make_pair(1.0f, 0.0f);
                    break;
            }
            break;
        case Orientation2D::DOWN:
            switch (corner) {
                case Corner2D::TOPLEFT:
                    return std::make_pair(0.0f, 0.0f);
                    break;
                case Corner2D::TOPRIGHT:
                    return std::make_pair(1.0f, 0.0f);
                    break;
                case Corner2D::BOTTOMLEFT:
                    return std::make_pair(0.0f, 1.0f);
                    break;
                case Corner2D::BOTTOMRIGHT:
                    return std::make_pair(1.0f, 1.0f);
                    break;
            }
            break;
        case Orientation2D::LEFT:
            switch (corner) {
                case Corner2D::TOPLEFT:
                    return std::make_pair(0.0f, 1.0f);
                    break;
                case Corner2D::TOPRIGHT:
                    return std::make_pair(1.0f, 1.0f);
                    break;
                case Corner2D::BOTTOMLEFT:
                    return std::make_pair(0.0f, 0.0f);
                    break;
                case Corner2D::BOTTOMRIGHT:
                    return std::make_pair(1.0f, 0.0f);
                    break;
            }
            break;
        case Orientation2D::RIGHT:
            switch (corner) {
                case Corner2D::TOPLEFT:
                    return std::make_pair(0.0f, 1.0f);
                    break;
                case Corner2D::TOPRIGHT:
                    return std::make_pair(1.0f, 1.0f);
                    break;
                case Corner2D::BOTTOMLEFT:
                    return std::make_pair(0.0f, 0.0f);
                    break;
                case Corner2D::BOTTOMRIGHT:
                    return std::make_pair(1.0f, 0.0f);
                    break;
            }
            break;
    }
    return std::make_pair(0.0f, 0.0f);
}

unsigned VoxelInfo::getTextureIndex(const unsigned int &id, const FaceDirection &face) {
	return voxelDataMap[id].faceTextures[face];
}

void VoxelInfo::setTextureIndex(const unsigned int &id, const FaceDirection &face, const unsigned int &index) {
	voxelDataMap[id].faceTextures[face] = index;
}

void VoxelInfo::setAllTextureIndexes(const unsigned int &id, const unsigned int &index) {
	auto& textures = voxelDataMap[id].faceTextures;
	textures[FaceDirection::TOP] = index;
	textures[FaceDirection::BOTTOM] = index;
	textures[FaceDirection::NORTH] = index;
	textures[FaceDirection::SOUTH] = index;
	textures[FaceDirection::EAST] = index;
	textures[FaceDirection::WEST] = index;
}

void VoxelInfo::setTextureData(const unsigned& id, const Face3D& face, const Orientation2D& orientation, const float& textureIndex) {
    voxelDataMap[id].faceOrientation[face] = orientation;
    voxelDataMap[id].faceTextureIndices[face] = textureIndex;
}

void VoxelInfo::setTransparent(const unsigned& id, const bool& transparent) {
    voxelDataMap[id].transparent = transparent;
}

bool VoxelInfo::isTransparent(const unsigned& id) {
    return voxelDataMap[id].transparent;
}

}

