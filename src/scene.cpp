/*
 * =====================================================================================
 *
 *       Filename:  scene.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/24/2014 04:10:57 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include "scene.h"
#include "game.h"

namespace vtk {

void Scene::link(Game* game) {
    linkedGame = game;
}

}
