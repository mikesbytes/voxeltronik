/*
 * =====================================================================================
 *
 *       Filename:  inputhandler.h
 *
 *    Description:  Passes input to an inputbridge
 *
 *        Version:  1.0
 *        Created:  03/28/2014 06:19:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#include <SDL2/SDL.h>
#include <map>
#include "nano_signal_slot.hpp"

namespace vtk {

struct ActionIdentifier {
    SDL_Keycode keyCode; //Key code
    unsigned mouseCode; //Mouse button code
    bool mouse; //Is mouse button?

};

class InputHandler {
public:
    void update();
    bool isActionDown(const std::string& action);
    bool setAction(const std::string& actionName, const std::string& actionButton);
    Nano::signal<void()>& getEventSignal(const Uint32& type);

protected:
    std::map<std::string, ActionIdentifier> actions;
    std::map<Uint32, Nano::signal<void()> > events; //SDL Event type handling
    Uint32 mouseButtons;
    const Uint8* keys;
    SDL_Event event;
};

}
