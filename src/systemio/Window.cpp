/*	Author: Matthew Littlemore	*/
/*	Purpose: Used to create and manage a window using GLFW	*/

#include <systemio/Window.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

namespace mathernogl {

Window::Window() {
	timeSinceLastSample = glfwGetTime();
	frameCount = 0;
	latestFPSMeasure = 0.0;
}

//will attempt to create a window with given dimensions and name
//throws runtime_error exceptions if fails
void Window::create(uint windowWidth, uint windowHeight, std::string windowName, bool fullscreen, bool antiAliasing, bool enableSRGB) {
	glfwSetTime(0.0);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, 0);
  glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);

	if (antiAliasing) {
		glfwWindowHint(GLFW_SAMPLES, 4);
	}

	GLFWmonitor* monitor = nullptr;
	if (fullscreen) {
		monitor = glfwGetPrimaryMonitor();
	}

	//create the window
	window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), monitor, nullptr);
	if (window == nullptr){
		throw std::runtime_error("Couldn't initialise GLFW window. ");
	}
	glfwSetWindowSize(window, windowWidth, windowHeight);
	glfwMakeContextCurrent(window);

	glfwSwapInterval(0);
  if (enableSRGB)
    glEnable(GL_FRAMEBUFFER_SRGB);

  glEnable(GL_MULTISAMPLE);
  glViewport(0, 0, windowWidth, windowHeight);

  imGuiInit();
}

//clears the colour and depth buffers of the window
void Window::clear() const {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//to be called to update the screen to draw the next frame
void Window::update() const {
	//swap the drawing and memory buffers
	glfwSwapBuffers(window);

	//update GLFW
	glfwPollEvents();
}

void Window::imGuiInit(){
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 130");
}

void Window::imGuiStartNewFrame(){
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void Window::imGuiRenderFrame(){
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::imGuiCleanUp(){
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

//will return true if the GLFW window is closed
bool Window::isOpen() const {
	return !glfwWindowShouldClose(window);
}

float Window::getAspectRatio() const {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return float(width) / float(height);
}

uint Window::getWidth() const {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return width;
}

uint Window::getHeight() const {
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return height;
}

Vector2D Window::getSize() const {
	return Vector2D(getWidth(), getHeight());
}

//will return an average fps value sampling every given number of frames
//must be called once per frame to get expected results
float Window::getFPS(int numSampleFrames) {
	frameCount++;
	if ((int) frameCount >= numSampleFrames) {
		latestFPSMeasure = frameCount / (glfwGetTime() - timeSinceLastSample);
		timeSinceLastSample = glfwGetTime();
		frameCount = 0;
	}
	return latestFPSMeasure;
}

void Window::setVSync(bool vsync) {
	if (vsync) {
		glfwSwapInterval(1);
	}
	else {
		glfwSwapInterval(0);
	}
}

void Window::setClearColour(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Window::close() {
    imGuiCleanUp();
    glfwDestroyWindow(window);
}

}
