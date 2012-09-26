#include <iostream>

#include "/Users/Dmitri/Dmitri/^Work/^Libraries/glfw-2.7.6/include/GL/glfw.h"

using namespace std;

int main(int argc, char * argv[])
{
	glfwInit();
	// Verify the GLFW library and header versions match
	{
		int Major, Minor, Revision;
		glfwGetVersion(&Major, &Minor, &Revision);

		bool Match = (GLFW_VERSION_MAJOR == Major && GLFW_VERSION_MINOR == Minor && GLFW_VERSION_REVISION == Revision);

		if (!Match)
		{
			std::cerr << "Error: GLFW library and header versions do not match." << std::endl;

			throw 0;
		}
		else
		{
			std::cout << "Using GLFW " << Major << "." << Minor << "." << Revision << "." << std::endl;
		}
	}

	glfwOpenWindow(640, 480, 8, 8, 8, 0, 0, 0, GLFW_WINDOW);

	{
		glfwSetWindowTitle("Conception");
		glfwSwapInterval(1);					// Set Vsync
		glfwDisable(GLFW_AUTO_POLL_EVENTS);

		glfwEnable(GLFW_KEY_REPEAT);
		glfwDisable(GLFW_SYSTEM_KEYS);
	}

	//system("/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/print-args I dont know\\ whatever");
	auto Result = system("/usr/local/go/bin/go run /Users/Dmitri/Desktop/launcher.go");
	std::cout << "System returns " << Result << endl;

	// Main loop
	while (glfwGetWindowParam(GLFW_OPENED))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// Process input
		{
			glfwPollEvents();
		}

		// Render
		{
			//MainApp.Render();
		}

		// Display new frame
		glfwSwapBuffers();

		// Use less CPU in background
		if (!glfwGetWindowParam(GLFW_ACTIVE))
			glfwSleep(0.100);
	}

	glfwTerminate();

	return 0;
}
