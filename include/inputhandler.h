#pragma once

//TODO: Tear out old signal/slot based input handling once
// everything is done via script

#include <SDL2/SDL.h>
#include <map>
#include "nano_signal_slot.hpp"

#include "sol.hpp"

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
	void setEventFn(const Uint32& type, sol::function f);


	static void registerScriptInterface(sol::state& lua);
protected:
    std::map<std::string, ActionIdentifier> actions;
    std::map<Uint32, Nano::signal<void()> > events; //SDL Event type handling
	std::map<Uint32, sol::function> mEventFns;
    Uint32 mouseButtons;
    const Uint8* keys;
    SDL_Event event;
    Sint32 mMouseXRel;
    Sint32 mMouseYRel;
};

}
