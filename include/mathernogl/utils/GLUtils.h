#pragma once
//
// Created by matt on 20/04/17.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <mathernogl/systemio/Window.h>

namespace mathernogl {

/*
 * initialises glew and glfw and creates a window with the given parameters
 * window must be destroyed and memory freed once finished with
 */
Window *initGL(std::string windowName, uint windowWidth, uint windowHeight, bool fullScreen = false, bool antiAliasing = false);

/*
 * sets the state of alpha blending
 * uses default alpha blending settings
 */
void setAlphaBlending(bool active);

/*
 * sets the state of face culling
 * uses back face culling is cullBackFace is true
 * uses ccw front face vertex order if ccwFrontFace is true
 */
void setFaceCulling(bool active, bool cullBackFace = true, bool ccwFrontFace = true);

/*
 * sets the state of the depth test
 */
void setDepthTest(bool active);
}