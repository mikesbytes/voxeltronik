/*
 * =====================================================================================
 *
 *       Filename:  voxelutils.h
 *
 *    Description:  Positioning and stuff.
 *
 *        Version:  1.0
 *        Created:  04/03/2014 07:27:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include <tuple>
#include <glm/glm.hpp>

namespace vtk {

typedef std::tuple<unsigned, unsigned, unsigned> uPos; //used for voxel positions

struct uPosHash : public std::unary_function<uPos, std::size_t> {
    std::size_t operator()(const uPos& k) const {
        return std::get<0>(k) ^ std::get<1>(k) ^ std::get<2>(k);
    }
};

struct uPosEqual : public std::binary_function<uPos, uPos, bool> {
    bool operator()(const uPos& a, const uPos& b) const {
        return (std::get<0>(a) == std::get<0>(b) &&
                std::get<1>(a) == std::get<1>(b) &&
                std::get<2>(a) == std::get<2>(b));
    }
};


typedef std::tuple<int, int, int> iPos; //used for chunk positions
struct iPosHash : public std::unary_function<iPos, std::size_t> {
    std::size_t operator()(const iPos& k) const {
        return std::get<0>(k) ^ std::get<1>(k) ^ std::get<2>(k);
    }
};

struct iPosEqual : public std::binary_function<iPos, iPos, bool> {
    bool operator()(const iPos& a, const iPos& b) const {
        return (std::get<0>(a) == std::get<0>(b) &&
                std::get<1>(a) == std::get<1>(b) &&
                std::get<2>(a) == std::get<2>(b));
    }
};

// basic hash function for glm::ivec3
struct ivec3Hash : public std::unary_function<iPos, std::size_t> {
  std::size_t operator()(const glm::ivec3& k) const {
        return k.x ^ k.y ^ k.z;
    }
};

/*
struct ivec3Equal : public std::binary_function<iPos, iPos, bool> {
    bool operator()(const iPos& a, const iPos& b) const {
        return (std::get<0>(a) == std::get<0>(b) &&
                std::get<1>(a) == std::get<1>(b) &&
                std::get<2>(a) == std::get<2>(b));
    }
};
*/

}
