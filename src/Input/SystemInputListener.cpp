#include "../Main.h"

#if 0
SystemInputListener::SystemInputListener()
	: InputListener()
{
}

SystemInputListener::~SystemInputListener()
{
}

void SystemInputListener::ProcessButton(InputManager::InputId ButtonId, bool Pressed)
{
	if (Pressed)
	{
		if (0 == ButtonId.Device)
		{
			switch (ButtonId.Id) {
			case GLFW_KEY_ESC:
				glfwCloseWindow();
				break;
			case GLFW_KEY_F5:
				// TODO: This doesn't belong in SystemInputListener; move it to proper place and make CurrentProject a non-global
				{
					CurrentProject.GenerateProgram();
					CurrentProject.RunProgram();
				}
				break;
			// TEST
			case 'B':
				if (glfwGetKey(GLFW_KEY_LCTRL) || glfwGetKey(GLFW_KEY_RCTRL))
				{
					CurrentProject.GetStdIncludes().push_back(FindConcept("test"));
				}
				break;
			// TEST
			case 'M':
				//if (glfwGetKey(GLFW_KEY_LCTRL) || glfwGetKey(GLFW_KEY_RCTRL))
				{
					//g_InputManager->SetMouseCursorVisibility(!g_InputManager->IsMouseCursorVisible());
					/*int xpos, ypos;
					glfwGetMousePos(&xpos, &ypos);
					glfwSetMousePos(xpos+1, ypos);*/
				}
				break;
			default:
				break;
			}
		}
	}
}
#endif
