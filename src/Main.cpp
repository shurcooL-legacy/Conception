/*
TODO so that Conception can be implemented within Conception:
 -Add/manipulate headers to project
 -Add/manipulate classes
 -Add/manipulate functions
 -Add/manipulate member variables, member functions
 -Compile project
 -Save/load project

TODO:
 -Remove InputListeners from InputManager (keep PointerInputListeners only)
*/

#include "Main.h"

#ifdef WIN32
namespace std
{
	sint32 lround(double arg)
	{
		return static_cast<sint32>(arg + 0.5);
	}
}
#endif // WIN32

// Main function
int main(int argc, char * argv[])
{
	/*{
		PointerState test;

		test.UpdateButtonState(0) = true;
		test.UpdateButtonState(1) = true;
		test.UpdateButtonState(2) = true;

		PointerState test2(test);

		test2.UpdateButtonState(2) = false;

		std::cout << test.GetButtonState(2) << &std::endl;
		std::cout << test2.GetButtonState(2) << &std::endl;

		return 0;
	}*/
#if 0
	{
		auto Test = []() { std::cout << "Hi from anon func.\n"; };

		// Call Test()
		Test();

		return 0;
	}
#endif








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

	// Open main window
	{
		GLFWvidmode DesktopMode;
		glfwGetDesktopMode(&DesktopMode);

		///glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 32);

		const bool Fullscreen = static_cast<bool>(0);
		const Vector2n WindowDimensons(1024, 768);

		if (!Fullscreen) {
			glfwOpenWindow(WindowDimensons.X(), WindowDimensons.Y(), DesktopMode.RedBits, DesktopMode.GreenBits, DesktopMode.BlueBits, 0, 0, 0, GLFW_WINDOW);
			glfwSetWindowPos((DesktopMode.Width - WindowDimensons.X()) / 2, (DesktopMode.Height - WindowDimensons.Y()) / 2);		// Center the window
		} else {
			glfwOpenWindow(DesktopMode.Width, DesktopMode.Height, DesktopMode.RedBits, DesktopMode.GreenBits, DesktopMode.BlueBits, 0, 0, 0, GLFW_FULLSCREEN);
			glfwEnable(GLFW_MOUSE_CURSOR);
		}

		{
			std::ostringstream x;
			x << "CPU Count: " << glfwGetNumberOfProcessors() << std::endl
			  << "GL Renderer: " << glGetString(GL_VENDOR) << " " << glGetString(GL_RENDERER) << " " << glGetString(GL_VERSION) << std::endl
			  << "GLFW_ACCELERATED: " << glfwGetWindowParam(GLFW_ACCELERATED) << std::endl
			  << "GLFW_RED_BITS: " << glfwGetWindowParam(GLFW_RED_BITS) << std::endl
			  << "GLFW_GREEN_BITS: " << glfwGetWindowParam(GLFW_GREEN_BITS) << std::endl
			  << "GLFW_BLUE_BITS: " << glfwGetWindowParam(GLFW_BLUE_BITS) << std::endl
			  << "GLFW_ALPHA_BITS: " << glfwGetWindowParam(GLFW_ALPHA_BITS) << std::endl
			  << "GLFW_DEPTH_BITS: " << glfwGetWindowParam(GLFW_DEPTH_BITS) << std::endl
			  << "GLFW_STENCIL_BITS: " << glfwGetWindowParam(GLFW_STENCIL_BITS) << std::endl
			  << "GLFW_REFRESH_RATE: " << glfwGetWindowParam(GLFW_REFRESH_RATE) << std::endl
			  << "GLFW_FSAA_SAMPLES: " << glfwGetWindowParam(GLFW_FSAA_SAMPLES) << std::endl;
			std::cout << x.str();
		}

		glfwSetWindowTitle("Conception");
		glfwSwapInterval(1);					// Set Vsync
		glfwDisable(GLFW_AUTO_POLL_EVENTS);

		glfwEnable(GLFW_KEY_REPEAT);
		glfwDisable(GLFW_SYSTEM_KEYS);
	}

	InputManager InputManager;
	g_InputManager = &InputManager;

	ConceptionApp MainApp(InputManager);
	//ConceptionTestApp MainApp(InputManager);
	//MultitouchTestApp MainApp(InputManager);
	//SentienceApp MainApp(InputManager);

	// Perform the layout of UI widgets
	MainApp.Layout();

	// OpenGL settings
	InitializeOpenGL();

	std::cout << std::endl;		// Done loading

	// Main loop
	while (glfwGetWindowParam(GLFW_OPENED))
	{
		// DEBUG: Moved to top of loop to enable debug printing from input handling code
		glClear(GL_COLOR_BUFFER_BIT);		// Clear frame

		double CurrentTime = glfwGetTime();
		static double LastTime = CurrentTime;
		double TimePassed = CurrentTime - LastTime;
		LastTime = CurrentTime;

		{
			//printf("TimePassed %f\n", TimePassed * 1000);
		}

		// Process input
		{
			glfwPollEvents();
			InputManager.ProcessTimePassed(TimePassed);
		}

		// Render
		{
			// DEBUG: Moved to top of loop to enable debug printing from input handling code
			///glClear(GL_COLOR_BUFFER_BIT);		// Clear frame

			MainApp.Render();
		}

		// Display new frame
		glfwSwapBuffers();
		//glFinish();

		///printf("%f ms frame\n", TimePassed * 1000);
		//glfwSleep(0.009);
	}

	// Clean up
#if defined(__APPLE__) && defined(__MACH__)
	//system("rm ./GenProgram.cpp ./GenProgram.command");		// Clean up temporary files
#endif
	OglUtilsKillFont();
	glfwTerminate();

	return 0;
}
