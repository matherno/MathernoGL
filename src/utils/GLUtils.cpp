//
// Created by matt on 20/04/17.
//

#include "utils/GLUtils.h"
#include <utils/Logging.h>

namespace mathernogl{

Window* initGL(std::string windowName, uint windowWidth, uint windowHeight, bool fullScreen, bool antiAliasing, bool enableSRGB) {
    Window *window = new Window();

    resetLog();
    if (!glfwInit()) {
        logError("GLFW couldn't initialise.");
        throw std::runtime_error("MathernoGL: Couldn't initialise GLFW");
    }
    else {
        window->create(windowWidth, windowHeight, windowName, fullScreen, antiAliasing, enableSRGB);
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            logError("GLEW couldn't initialise. ");
            throw std::runtime_error("MathernoGL: Couldn't initialise GLEW");
        } else {
            glGetError();
            std::string version = (char *) glGetString(GL_VERSION);
            logInfo("Using OpenGL version: " + version);
        }
    }

    return window;
}

void setAlphaBlending(bool active, bool additive) {
    if(active) {
        glEnable(GL_BLEND);
        if (additive)
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);      // doesn't seems to work????
        else
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else{
        glDisable(GL_BLEND);
    }
}

void setFaceCulling(bool active, bool cullBackFace, bool ccwFrontFace) {
    if(active){
        glEnable(GL_CULL_FACE);
        if(cullBackFace){
            glCullFace(GL_BACK);
        }
        else{
            glCullFace(GL_FRONT);
        }
        if(ccwFrontFace){
            glFrontFace(GL_CCW);
        }
        else{
            glFrontFace(GL_CW);
        }
    }
    else{
        glDisable(GL_CULL_FACE);
    }
}

void setDepthTest(bool active) {
    if(active){
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }
    else{
        glDisable(GL_DEPTH_TEST);
    }
}

void clearGLErrors(){
  while(glGetError() != 0);
}

}