#include "app_wrap.h"

#ifdef _WIN32
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

GLFWwindow* gWindow;

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}


int appInit(AppInit init)
{
	// Setup window
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		return 1;

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (init.antialiasing != 0) glfwWindowHint(GLFW_SAMPLES, init.antialiasing);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	//glfwWindowHint(GLFW_ALPHA_BITS, GL_TRUE);
	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	gWindow = glfwCreateWindow(init.res_x, init.res_y, init.title, init.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	glfwSetWindowPos(gWindow, int(mode->width*init.pos_x), int(mode->height*init.pos_y));
	glfwSwapInterval(init.v_sync ? 1 : 0);

	if (!gWindow)
		return 1;
	glfwMakeContextCurrent(gWindow);
	gl3wInit();
	return 0;
}

void appGetState(AppState& state)
{
	glfwGetFramebufferSize(gWindow, &state.res_x, &state.res_y);
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	state.refresh_rate = mode->refreshRate; //#DODGY we are basically assuming that the primary monitor's windows request succeeded
}

bool appShouldClose()
{
	return (glfwWindowShouldClose(gWindow) != 0) || glfwGetKey(gWindow, GLFW_KEY_ESCAPE);
}

void appSwapBuffers()
{
	glfwSwapBuffers(gWindow);
}

void appTerm()
{
	glfwTerminate();
}

#endif


















#if __APPLE__
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif