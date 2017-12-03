#pragma once
//
// Created by matt on 27/03/17.
//


#include <GLFW/glfw3.h>
#include <algorithm>
#include <vector>

#include <mathernogl/maths/Vector2D.h>

namespace mathernogl {

enum CursorMode {
    CURSOR_ENABLED,		//normal cursor
    CURSOR_HIDDEN,		//hidden when above window
    CURSOR_DISABLED		//hidden and locked within window
};

enum InputAction{
    INPUT_HELD = 9,
    INPUT_PRESSED = GLFW_PRESS,
    INPUT_RELEASED = GLFW_RELEASE
};

struct KeyEvent {
    int key;
    InputAction action;
};

struct MouseButtonEvent {
    int buttonNum;
    InputAction action;
};

class InputHandler {
private:
    GLFWwindow* window;
    std::vector<KeyEvent> keyEvents;
    std::vector<MouseButtonEvent> mouseButtonEvents;
    double scrollWheelOffset = 0;
    std::vector<int> heldKeys;
    std::vector<int> heldMouseButtons;
    Vector2D mouseOffset = Vector2D(0);
    Vector2D lastMousePos = Vector2D(0);
    bool gotLastMousePos = false;

public:
    /*
     * initialises the input handler, must be called before anything else
     * given glfw window must have been initialised
     */
    void init(GLFWwindow* window);

    /*
     * cleans up the input handler
     */
    void cleanUp();

    /*
     * sets the cursor mode, either enabled, hidden, or disabled
     */
    void setCursorMode(CursorMode mode);

    /*
     * checks the state of the buttons to configure whether any are being held or not
     * should be called at the start of a step in the frame loop
     */
    void checkHeldButtons();

    /*
     * returns true if given key/mouse button action is in the current events
     */
    bool isKeyAction(int key, InputAction action) const;
    bool isMouseButtonAction(int buttonNum, InputAction action) const;

    /*
     * returns a list of active events
     * either from key board or mouse buttons
     */
    const std::vector<KeyEvent>* getKeyEvents() const;
    const std::vector<MouseButtonEvent>* getMouseButtonEvents() const;

    /*
     * returns the scroll wheel offset from the last event poll
     */
    double getScrollWheelOffset() const;

    /*
     * returns the mouse offset from the last event poll
     */
    Vector2D getMouseOffset() const;

    /*
     * obtains the current mouse position
     * top left is (0, 0)
     * relative mouse position has a bottom right of (1, 1)
     */
    Vector2D getMousePos() const;
    Vector2D getRelativeMousePos() const;

    /*
     * clears the currently stored events, ready for the next frame loop
     * should be called at the end of a step in the frame loop
     */
    void clearEvents();

    /*
     * functions used by static callback functions to update the state of this input handler
     * DO NOT USE
     */
    void processKey(int key, int glfwAction);
    void processMouseButton(int button, int glfwAction);
    void processScrollWheel(double yOffset);
    void processMouseOffset(const Vector2D &mousePos);

};
}


