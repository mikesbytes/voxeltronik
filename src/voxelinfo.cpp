#include "voxelinfo.h"
#include "world.h"

#include "sol.hpp"

namespace vtk {

VoxelInfo::VoxelInfo() :
	mHighestID(0)
{
	VoxelType air;
	air.tag = "air";
	air.name = "Air";
	air.transparent = true;
	air.emission = 0;
	mVoxelTypes[0] = air;
	mVoxelTags["air"] = 0; 
}

bool VoxelInfo::tagExists(const std::string &tag) {
	return mVoxelTags.find(tag) != mVoxelTags.end();
}

unsigned VoxelInfo::idFromTag(const std::string& tag) {
	auto s = mVoxelTags.find(tag);
	if (s == mVoxelTags.end()) return 0;
	return s->second;
}

bool VoxelInfo::newType(sol::table table) {
	unsigned newID = ++mHighestID;
	VoxelType newType;
	newType.tag = table.get_or<std::string>("tag", "");
	if (newType.tag == "") return false;

	newType.name = table.get_or<std::string>("name", "");
	//newType.transparent = table.get_or<bool>("transparent", (bool)false);
	auto trans = table.get<sol::optional<bool>>("transparent");
	if(trans) {
		newType.transparent = trans.value();
	} else {
		newType.transparent = false;
	}
	
	newType.emission = table.get_or<unsigned short>("emission", 0x0);

	sol::table texTab = table["textures"];
	auto allTex = texTab.get<sol::optional<unsigned>>("all");
	if (allTex) {
		for (int i = 0; i < 6; ++i) {
			newType.faceTextures[i] = allTex.value();
		}
	}
	auto x = texTab.get<sol::optional<unsigned>,
	                    sol::optional<unsigned>,
	                    sol::optional<unsigned>,
	                    sol::optional<unsigned>,
	                    sol::optional<unsigned>,
	                    sol::optional<unsigned>>
		("top", "bottom", "north", "south", "east", "west");
	if (std::get<0>(x))	newType.faceTextures[0] = std::get<0>(x).value();
	if (std::get<1>(x))	newType.faceTextures[1] = std::get<1>(x).value();
	if (std::get<2>(x))	newType.faceTextures[2] = std::get<2>(x).value();
	if (std::get<3>(x))	newType.faceTextures[3] = std::get<3>(x).value();
	if (std::get<4>(x))	newType.faceTextures[4] = std::get<4>(x).value();
	if (std::get<5>(x))	newType.faceTextures[5] = std::get<5>(x).value();

	//set the type if all is well
	mVoxelTypes[newID] = newType;
	mVoxelTags[newType.tag] = newID;
	return true;
}

VoxelType& VoxelInfo::getTypeByID(const unsigned& id) {
	return mVoxelTypes[id];
}

VoxelType& VoxelInfo::getTypeByTag(const std::string &tag) {
	return mVoxelTypes[mVoxelTags[tag]];
}

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
	return mVoxelTypes[id].faceTextures[static_cast<unsigned>(face)];
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
    mVoxelTypes[id].transparent = transparent;
}

bool VoxelInfo::isTransparent(const unsigned& id) {
    return mVoxelTypes[id].transparent;
}

void VoxelInfo::setEmission(const unsigned int &id, const unsigned short &emission) {
	voxelDataMap[id].emission = emission;
}

unsigned short VoxelInfo::getEmission(const unsigned int &id) {
	return mVoxelTypes[id].emission;
}

void VoxelInfo::registerScriptInterface(::sol::state &lua) {
	lua.new_usertype<VoxelInfo>("VoxelInfo",
	                            "new_type", &VoxelInfo::newType,
	                            "get_type_by_id", &VoxelInfo::getTypeByID,
	                            "get_type_by_tag", &VoxelInfo::getTypeByTag,
	                            "set_all_texture_indexes", &VoxelInfo::setAllTextureIndexes,
	                            "set_texture_index", &VoxelInfo::setTextureIndex,
	                            "set_transparent", &VoxelInfo::setTransparent,
	                            "set_emission", &VoxelInfo::setEmission);

	lua.new_usertype<VoxelType>("VoxelType",
	                            "tag", &VoxelType::tag,
	                            "name", &VoxelType::name);
}
}

