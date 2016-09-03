/*
 * =====================================================================================
 *
 *       Filename:  tileset.h
 *
 *    Description:  Tileset using OpenGL array texture
 *
 *        Version:  1.0
 *        Created:  04/02/2014 10:37:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include <map>

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

protected:
    std::map<std::string, std::string> textureFiles; //filename, texturename
    std::map<std::string, int> textureIndexes; //texturename, index

    int tilesize;
    int textureIndex;
    int maxTextures;
    unsigned texture; //OGL texture
};

}
