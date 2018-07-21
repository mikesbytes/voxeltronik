#pragma once

#include "voxelutils.h"
#include "sol.hpp"

#include <map>
#include <vector>
#include <array>
#include <glm/glm.hpp>

namespace sol {
class state;
}

namespace vtk {

enum class Corner2D {
    TOPLEFT,
    TOPRIGHT,
    BOTTOMLEFT,
    BOTTOMRIGHT
};

enum class Face3D {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    FRONT,
    BACK
};

enum class Orientation2D {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct VoxelData{
    std::map<Face3D, float> faceTextureIndices;
    std::map<Face3D, Orientation2D> faceOrientation;
	std::map<FaceDirection, unsigned> faceTextures;

    bool transparent;
	unsigned short emission;
};

struct VoxelType{
	std::string tag;
	std::string name;
	bool transparent;
	unsigned short emission;

	std::array<unsigned, 6> faceTextures;
	std::array<unsigned, 6> faceOrientations;
};

class World;

class VoxelInfo {
public:
	VoxelInfo();
	bool tagExists(const std::string& tag);
	unsigned idFromTag(const std::string& tag);
	bool newType(sol::table table);

	VoxelType& getTypeByID(const unsigned& id);
	VoxelType& getTypeByTag(const std::string& tag);	
	
    void pushTexCoordFromWorldCoords(std::vector<float>& data, const glm::ivec3& pos, const Face3D& face, const Corner2D& corner);
    void pushTexCoordFromVoxelID(std::vector<float>& data, const unsigned& id, const Face3D& face, const Corner2D& corner);
    float getTexIndexFromID(const unsigned& id, const Face3D& face);
	unsigned getTextureIndex(const unsigned& id, const FaceDirection& face);
	void setTextureIndex(const unsigned& id, const FaceDirection& face, const unsigned& index);

	void setAllTextureIndexes(const unsigned& id, const unsigned& index);
    
    void setTextureData(const unsigned& id, const Face3D& face, const Orientation2D& orientation, const float& textureIndex);

    void setTransparent(const unsigned& id, const bool& transparent);
	bool isTransparent(const unsigned& id);

    std::pair<float, float> getTexCoordFromID(const unsigned& id, const Face3D& face, const Corner2D& corner);

	void setEmission(const unsigned& id, const unsigned short& emission);
	unsigned short getEmission(const unsigned& id);

    World* linkedWorld;

	static void registerScriptInterface(::sol::state &lua);
protected:
	unsigned mHighestID;
	std::map<unsigned, VoxelType> mVoxelTypes;
	std::map<std::string, unsigned> mVoxelTags; //tag map for quick id lookups
    std::map<unsigned, VoxelData> voxelDataMap;
};

}
