//
// Created by matt on 20/04/17.
//

#include "mathernogl/utils/GLUtils.h"
#include <mathernogl/utils/Logging.h>

namespace mathernogl{

Window* initGL(std::string windowName, uint windowWidth, uint windowHeight, bool fullScreen, bool antiAliasing) {
    Window *window = new Window();

    resetLog();
    if (!glfwInit()) {
        logError("GLFW couldn't initialise.");
        throw std::runtime_error("MathernoGL: Couldn't initialise GLFW");
    }
    else {
        window->create(windowWidth, windowHeight, windowName, fullScreen, antiAliasing);
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

void setAlphaBlending(bool active) {
    if(active) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
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

}