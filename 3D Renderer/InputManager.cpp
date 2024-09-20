//
//  InputManager.cpp
//  3D Renderer
//
//  Created by Madeline Simpson on 9/19/24.
//

#include "InputManager.h"

InputManager::InputManager() : mouseX(0), mouseY(0) {}

InputManager::~InputManager() {
    // Cleanup if needed
}


void InputManager::mouseMoved(float x, float y) {
    mouseX = x;
    mouseY = y;
}

float InputManager::getMouseX() const {
    return mouseX;
}

float InputManager::getMouseY() const {
    return mouseY;
}
