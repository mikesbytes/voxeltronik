/*
 * =====================================================================================
 *
 *       Filename:  config.cpp
 *
 *    Description:  Specializations and shit
 *
 *        Version:  1.0
 *        Created:  03/27/2014 10:56:51 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "config.h"

template <>
std::string Config::getValue<std::string>(const std::string& key, const std::string& fallback) {
    if (values.find(key) != values.end()) {
        return values[key];
    }
    return fallback;
}

