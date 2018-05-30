/*
 * =====================================================================================
 *
 *       Filename:  mathplus.h
 *
 *    Description:  Some extra math functions I use
 *
 *        Version:  1.0
 *        Created:  04/06/2014 05:52:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <cmath>

namespace mp {

inline constexpr int floor_div3(int a, int b) {
	int d = a / b;
	return d * b == a ? d : d - ((a < 0) ^ (b < 0));
}

//Convert value to one within min/max. Useful for angles n' shit
template <typename T>
T normalize(const T& value, const T& min, const T& max) {
    return fmod((value - min), (max - min)) + min;
}

//Proportional normalization
template <typename T>
T scale(const T& value, const T& min, const T& max) {
    return (value - min) / (max - min);
}

//get sign of number
template <typename T> int signum(const T& val) {
    return (T(0) < val) - (val < T(0));
}

//floating point modulus
template <typename T> T mod(const T& a, const T& b) {
    return fmod(fmod(a,b) + b, b);
}

// constrain a value to bounds
template <typename T>
T constrain(const T& value, const T& min, const T& max) {
    if (value <= min) return min;
    if (value >= max) return max;
    return value;
}

template <typename T> T bound(const T& s, const T& ds) {
    if (ds < 0) {
        return bound(-s, -ds);
    }

    double ns = mod(s, (T)1);
    return (1-ns)/ds;
}
}
