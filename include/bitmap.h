/*
 * =====================================================================================
 *
 *       Filename:  bitmap.h
 *
 *    Description:  Stores a bitmap
 *
 *        Version:  1.0
 *        Created:  04/02/2014 07:19:09 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include <string>

namespace vtk {

class Bitmap {
public:
    Bitmap();

    bool loadFromFile(const std::string& fileName);
    unsigned char* getPixelDataPtr();
    int getHeight();
    int getWidth();
    int getNumChannels();
    void flipVertical();

protected:
    unsigned char* imageData;
    int x, y, n;
    int forceChannels;
};

}
