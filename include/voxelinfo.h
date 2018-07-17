#pragma once

#include "voxelutils.h"

#include <map>
#include <vector>
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

class World;

class VoxelInfo {
public:
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

    std::map<unsigned, VoxelData> voxelDataMap;
};

}
