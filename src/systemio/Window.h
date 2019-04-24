/*	Author: Matthew Littlemore	*/
/*	Purpose: Used to create and manage a window using GLFW	*/

#pragma once

#include <iostream>
#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <maths/Maths.h>
#include <Types.h>

namespace mathernogl {

class Window {
private:
	GLFWwindow* window;
	double timeSinceLastSample;	//used for fps counting
	unsigned int frameCount;		//		"		"
	float latestFPSMeasure;		//		"		"
public:
	Window();

	void create(uint windowWidth, uint windowHeight, std::string windowName, bool fullscreen, bool antiAliasing, bool enableSRGB = false);
  void close();
  void clear() const;
	void update() const;
  void imGuiStartNewFrame();
  void imGuiRenderFrame();
	bool isOpen() const;
	GLFWwindow* getGLFWWindow() const { return window; }
	float getAspectRatio() const;
	uint getWidth() const;
	uint getHeight() const;
	Vector2D getSize() const;
	void setVSync(bool vsync);
  void setClearColour(float r, float g, float b, float a = 1.0);
	float getFPS(int numSampleFrames);

private:
  void imGuiInit();
  void imGuiCleanUp();
};

}

