#include "tileset.h"
#include "bitmap.h"
#include "sol.hpp"

#include <SDL2/SDL_opengl.h>
#include <iostream>

namespace vtk {

Tileset::Tileset() {
    tilesize = 32;
    maxTextures = 10;
    textureIndex = 0;
}

void Tileset::bind() {
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
}

bool Tileset::addTexture(const std::string& fileName, const std::string& textureName) {
    textureFiles[fileName] = textureName;
    return true;
}

void Tileset::updateTextureAt(const int& index, const std::string& fileName) {
    Bitmap bitmap;
    bitmap.loadFromFile(fileName);
    glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, index, tilesize, tilesize, 1, GL_RGBA,
                GL_UNSIGNED_BYTE, bitmap.getPixelDataPtr());
}

int Tileset::getTextureIndex(const std::string& textureName) {
    if (textureIndexes.find(textureName) == textureIndexes.end()) {
        textureIndexes[textureName] = textureIndex;
        textureIndex++;
    }
    return textureIndexes[textureName];
}

void Tileset::buildTexture() {
    textureIndexes.clear();
    textureIndex = 0;

    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, 
                 tilesize, 
                 tilesize, 
                 maxTextures, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 NULL);

    glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}

void Tileset::deleteTexture() {

}

void Tileset::registerScriptInterface(::sol::state &lua) {
	lua.new_usertype<Tileset>("Tileset",
	                          "bind", &Tileset::bind,
	                          "update_texture_at", &Tileset::updateTextureAt,
	                          "get_texture_index", &Tileset::getTextureIndex,
	                          "build_texture", &Tileset::buildTexture);
}

}
