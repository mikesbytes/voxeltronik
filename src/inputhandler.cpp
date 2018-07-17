#include "inputhandler.h"
#include <iostream>
namespace vtk {

void InputHandler::update() {
    while (SDL_PollEvent(&event) ) {
        if (events.find(event.type) != events.end()) {
            events[event.type]();
        }
        if (mEventFns.find(event.type) != mEventFns.end()) {
            mEventFns[event.type]();
        }
    }
    mouseButtons = SDL_GetMouseState(NULL, NULL);
    keys = SDL_GetKeyboardState(NULL);
}

bool InputHandler::isActionDown(const std::string& action) {
    ActionIdentifier& curAction = actions[action];
    if (curAction.mouse) {
        switch (curAction.mouseCode) {
        case 1:
            return mouseButtons&SDL_BUTTON(1);
            break;
        case 2:
            return mouseButtons&SDL_BUTTON(2);
            break;
        case 3:
            return mouseButtons&SDL_BUTTON(3);
            break;
        case 4:
            return mouseButtons&SDL_BUTTON(4);
            break;
        case 5:
            return mouseButtons&SDL_BUTTON(5);
            break;
        case 6:
            return mouseButtons&SDL_BUTTON(6);
            break;
        case 7:
            return mouseButtons&SDL_BUTTON(7);
            break;
        default:
            break;
        }
    } else {
        return keys[SDL_GetScancodeFromKey(curAction.keyCode)];
    }
    return false;
}

bool InputHandler::setAction(const std::string& actionName, const std::string& actionButton) {
    ActionIdentifier newIdentifier;
    if (actionButton.find("Mouse ") != std::string::npos) {
        newIdentifier.mouse = true;
        if (actionButton == "Mouse Left") {
            newIdentifier.mouseCode = 1;
        } else if (actionButton == "Mouse Middle") {
            newIdentifier.mouseCode = 2;
        } else if (actionButton == "Mouse Right") {
            newIdentifier.mouseCode = 3;
        } else if (actionButton == "Mouse Wheel Up") {
            newIdentifier.mouseCode = 4;
        } else if (actionButton == "Mouse Wheel Down") {
            newIdentifier.mouseCode = 5;
        } else if (actionButton == "Mouse Shoulder 1") {
            newIdentifier.mouseCode = 6;
        } else if (actionButton == "Mouse Shoulder 2") {
            newIdentifier.mouseCode = 7;
        }
    } else {
        newIdentifier.mouse = false;
        newIdentifier.keyCode = SDL_GetKeyFromName(actionButton.c_str());
    }
    actions[actionName] = newIdentifier;
    return true;
}

void InputHandler::setEventFn(const Uint32 &type, sol::function f) {
	mEventFns[type] = f;
}

Nano::signal<void()>& InputHandler::getEventSignal(const Uint32& type) {
    return events[type];
}

void InputHandler::registerScriptInterface(sol::state &lua) {
	lua.new_usertype<InputHandler>("InputHandler",
	                               "update", &InputHandler::update,
	                               "is_action_down", &InputHandler::isActionDown,
	                               "set_action", &InputHandler::setAction,
	                               "set_event_fn", &InputHandler::setEventFn);
	                               
}
}
