/*
 * =====================================================================================
 *
 *       Filename:  bitmap.cpp
 *
 *    Description:  Bitmap
 *
 *        Version:  1.0
 *        Created:  04/02/2014 07:21:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "bitmap.h"
#include "stb_image.h"

namespace vtk {

Bitmap::Bitmap() {
    forceChannels = 4;
}

bool Bitmap::loadFromFile(const std::string& fileName) {
    imageData = stbi_load(fileName.c_str(), &x, &y, &n, forceChannels);
    if (!imageData) return false;
    flipVertical();
    return true;
}

unsigned char* Bitmap::getPixelDataPtr() {
    return imageData;
}

void Bitmap::flipVertical() {
    int widthBytes = x * 4;
    unsigned char* top = NULL;
    unsigned char* bottom = NULL;
    unsigned char temp = 0;
    int halfHeight = y/2;

    for (int row = 0; row < halfHeight; row++) {
        top = imageData + row * widthBytes;
        bottom = imageData + (y - row - 1) * widthBytes;
        for (int col = 0; col < widthBytes; col++) {
            temp = *top;
            *top = *bottom;
            *bottom = temp;
            top++;
            bottom++;
        }
    }
}

int Bitmap::getHeight() { return y; }
int Bitmap::getWidth() { return x; }
int Bitmap::getNumChannels() { return n; }

}
