//
// Created by matt on 27/03/17.
//

#include "mathernogl/systemio/InputHandler.h"
#include "mathernogl/utils/Logging.h"

namespace mathernogl{

namespace {
    InputHandler* inputHandler;	//provides a means for the static callback functions to access the InputHandler object, should only need one throughout system
}

void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void mousePosCallback(GLFWwindow* window, double xPos, double yPos);

Vector2D glfwGetMousePos(GLFWwindow* window){
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    return Vector2D(xPos, yPos);
}

void InputHandler::init(GLFWwindow* window) {
    inputHandler = this;    //so the static functions below have access to this object
    this->window = window;

    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);
    glfwSetCursorPosCallback(window, mousePosCallback);
}

void InputHandler::cleanUp() {
    clearEvents();
    heldKeys.clear();
    heldMouseButtons.clear();
    lastMousePos = Vector2D(0);
}

//can be used to set the cursor mode -> enabled, hidden, or disabled
void InputHandler::setCursorMode(CursorMode mode) {
    if (mode == CURSOR_ENABLED)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else if (mode == CURSOR_HIDDEN)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    else if (mode == CURSOR_DISABLED)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputHandler::checkHeldButtons() {
    for (int keyNum : heldKeys) {
        KeyEvent keyEvent;
        keyEvent.action = INPUT_HELD;
        keyEvent.key = keyNum;
        keyEvents.push_back(keyEvent);
    }

    for (int mouseNum : heldMouseButtons) {
        MouseButtonEvent mouseButtonEvent;
        mouseButtonEvent.buttonNum = mouseNum;
        mouseButtonEvent.action = INPUT_HELD;
        mouseButtonEvents.push_back(mouseButtonEvent);
    }
}

const std::vector<KeyEvent>* InputHandler::getKeyEvents() const {
    return &keyEvents;
}

const std::vector<MouseButtonEvent>* InputHandler::getMouseButtonEvents() const {
    return &mouseButtonEvents;
}

double InputHandler::getScrollWheelOffset() const {
    return scrollWheelOffset;
}

Vector2D InputHandler::getMousePos() const {
    if(gotLastMousePos){
        return lastMousePos;
    }
    else{
        return glfwGetMousePos(window);
    }
}

Vector2D InputHandler::getRelativeMousePos() const {
    Vector2D mousePos = getMousePos();
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return Vector2D(mousePos.x/(float)width, mousePos.y/(float)height);
}

Vector2D InputHandler::getMouseOffset() const {
    return mouseOffset;
}

void InputHandler::clearEvents() {
    keyEvents.clear();
    mouseButtonEvents.clear();
    scrollWheelOffset = 0;
    mouseOffset = Vector2D(0);
}

//TODO make more efficient with a std::map
bool InputHandler::isKeyAction(int key, InputAction action) const {
    for(const KeyEvent keyEvent : keyEvents){
        if(keyEvent.key == key && keyEvent.action == action){
            return true;
        }
    }
    return false;
}

//TODO make more efficient with a std::map
bool InputHandler::isMouseButtonAction(int buttonNum, InputAction action) const {
    for(const MouseButtonEvent mouseEvent : mouseButtonEvents){
        if(mouseEvent.buttonNum == buttonNum && mouseEvent.action == action){
            return true;
        }
    }
    return false;
}

//called by the callback functions to process a key event
void InputHandler::processKey(int key, int glfwAction) {
    if (glfwAction == GLFW_PRESS) {
        heldKeys.push_back(key);
    }
    else if (glfwAction == GLFW_RELEASE) {
        heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), key), heldKeys.end());
    }

    if(glfwAction != GLFW_REPEAT) {
        KeyEvent keyEvent;
        keyEvent.action = (InputAction) glfwAction;
        keyEvent.key = key;
        keyEvents.push_back(keyEvent);
    }
}

//called by the callback functions to process a mouse button event
void InputHandler::processMouseButton(int button, int glfwAction) {
    if (glfwAction == INPUT_PRESSED) {
        heldMouseButtons.push_back(button);
    }
    else if (glfwAction == INPUT_RELEASED) {
        heldMouseButtons.erase(std::remove(heldMouseButtons.begin(), heldMouseButtons.end(), button), heldMouseButtons.end());
    }

    MouseButtonEvent mouseButtonEvent;
    mouseButtonEvent.buttonNum = button;
    mouseButtonEvent.action = (InputAction) glfwAction;
    mouseButtonEvents.push_back(mouseButtonEvent);
}

//called by the callback functions to process a mouse scroll event
void InputHandler::processScrollWheel(double yOffset) {
    scrollWheelOffset += yOffset;
}

//called by the callback functions to process a mouse offset event
void InputHandler::processMouseOffset(const Vector2D &mousePos) {
    if(!gotLastMousePos){
        lastMousePos = mousePos;
        gotLastMousePos = true;
    }
    this->mouseOffset = mousePos - lastMousePos;
    lastMousePos = mousePos;
}

//callback functions that glfw will call for input events

void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods) {
    inputHandler->processKey(key, action);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    inputHandler->processMouseButton(button, action);
}

void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    inputHandler->processScrollWheel(yOffset);
}

void mousePosCallback(GLFWwindow *window, double xPos, double yPos) {
    inputHandler->processMouseOffset(Vector2D(xPos, yPos));
}

}
