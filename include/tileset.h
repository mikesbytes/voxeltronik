#pragma once

#include <map>

namespace sol {
class state;
}

namespace vtk {

class Tileset {
public:
    Tileset();

    void bind(); //Bind to active texture
    bool addTexture(const std::string& fileName, const std::string& textureName);
    void updateTextureAt(const int& index, const std::string& fileName);
    int getTextureIndex(const std::string& textureName);
    void buildTexture();
    void deleteTexture();

	static void registerScriptInterface(::sol::state& lua);
protected:
    std::map<std::string, std::string> textureFiles; //filename, texturename
    std::map<std::string, int> textureIndexes; //texturename, index

    int tilesize;
    int textureIndex;
    int maxTextures;
    unsigned texture; //OGL texture
};

}
