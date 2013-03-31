#include "../Main.h"

InputManager * g_InputManager = nullptr;

InputManager * InputManager::m_pInstance = nullptr;

InputManager::InputManager()
	: m_TypingPointer(),
	  m_MousePointer(),
	  m_TouchPointers(),
	  m_InputEventQueue(),
	  m_InputHandler(nullptr),
	  m_MouseCursorVisible(true),
	  m_MouseIgnorePositionOnce(false),
	  m_MouseIgnorePositionAlways(false),
	  m_WindowDimensions()
{
	assert(nullptr == m_pInstance);		// m_pInstance wasn't == nullptr, means we have more than 1 instance of InputManager, not right
	m_pInstance = this;
}

InputManager::~InputManager()
{
	printf("~InputManager()\n");

	ClearInputHandler();		// THINK: Do I really need it call it a second time "just in case"?

	m_pInstance = nullptr;
}

// TODO: Clean this up... this function is legacy for external input handler setting, not needed if I'll be using internal input handler (InputEventQueue).
void InputManager::SetInputHandler(InputHandler * InputHandler)
{
	assert(nullptr == m_InputHandler);
	m_InputHandler = &m_InputEventQueue;
	//m_InputHandler = InputHandler;

	SetGlfwCallbacks();
}

void InputManager::ClearInputHandler()
{
	// Clear all pointer mappings
	if (nullptr != m_TypingPointer)
	{
		m_TypingPointer->ModifyPointerMapping().RemoveAllMappings();
	}
	if (nullptr != m_MousePointer)
	{
		m_MousePointer->ModifyPointerMapping().RemoveAllMappings();
	}
	for (auto & TouchPointer : m_TouchPointers)
	{
		TouchPointer.second->ModifyPointerMapping().RemoveAllMappings();
	}

	RemoveGlfwCallbacks();

	SetMouseCursorVisibility(true);
}

void InputManager::SetMouseCursorVisibility(bool Visible)
{
	static int MouseCursorDesktopPositionX, MouseCursorDesktopPositionY;

	// Hide Mouse Cursor
	if (!Visible && m_MouseCursorVisible)
	{
		glfwPollEvents();
		m_MouseIgnorePositionAlways = true;
		glfwGetMousePos(&MouseCursorDesktopPositionX, &MouseCursorDesktopPositionY);
		glfwDisable(GLFW_MOUSE_CURSOR);
		m_MouseIgnorePositionOnce = true;
		m_MouseIgnorePositionAlways = false;

		m_MouseCursorVisible = false;
	}
	// Show Mouse Cursor
	else if (Visible && !m_MouseCursorVisible)
	{
		m_MouseIgnorePositionAlways = true;
		glfwEnable(GLFW_MOUSE_CURSOR);
		glfwSetMousePos(MouseCursorDesktopPositionX, MouseCursorDesktopPositionY);
		m_MouseIgnorePositionOnce = true;
		m_MouseIgnorePositionAlways = false;

		m_MouseCursorVisible = true;

		// Create a mouse position event
		//ProcessMousePos(m_MousePositionX, m_MousePositionY);
	}
}

bool InputManager::IsMouseCursorVisible()
{
	return m_MouseCursorVisible;
}

Vector2n InputManager::GetWindowDimensions()
{
	return m_WindowDimensions;
}

void InputManager::RequestTypingPointer(GestureRecognizer & Target)
{
	// DECISION: Only allow pointer change when it's not captured
	if (nullptr == m_TypingPointer->GetPointerMapping().GetCapturer())
	//if (true)
	{
#if 0
		m_TypingPointer->ModifyPointerMapping().RemoveAllMappings();
		m_TypingPointer->ModifyPointerMapping().AddMapping(Target);
		m_TypingPointer->ModifyPointerMapping().DoneAdding();
#else
		m_TypingPointer->ModifyPointerMapping().RemoveAllMappings();
		if (nullptr != &Target)		// HACK: I should use pointer rather than reference (but too lazy to manually change all invokations...)
		{
			m_TypingPointer->ModifyPointerMapping().AddMapping(Target);
			for (auto Parent = static_cast<Widget *>(&Target.GetOwner())->ModifyParent(); nullptr != Parent; Parent = Parent->ModifyParent())
			{
				m_TypingPointer->ModifyPointerMapping().AddMapping(Parent->ModifyGestureRecognizer());
			}
		}
		m_TypingPointer->ModifyPointerMapping().DoneAdding();
#endif
	}
}

// Returns true if any of the pointers are active
bool InputManager::AnyActivePointers() const
{
	// TODO: All pointers, not just keyboard
	return m_TypingPointer->GetPointerState().IsAnyButtonsPressed();
}

bool InputManager::EmptyInputEventQueue() const
{
	return InputEventQueue().GetQueue().empty();
}

void InputManager::SetGlfwCallbacks()
{
	m_TypingPointer = std::unique_ptr<TypingPointer>(new TypingPointer(*m_InputHandler));
	m_MousePointer = std::unique_ptr<MousePointer>(new MousePointer(*m_InputHandler));

	glfwSetWindowSizeCallback(&InputManager::ProcessWindowSize);

	glfwSetKeyCallback(&InputManager::ProcessKey);
	glfwSetCharCallback(&InputManager::ProcessChar);
	glfwSetMouseButtonCallback(&InputManager::ProcessMouseButton);
	glfwSetMousePosCallback(&InputManager::ProcessMousePos);
	glfwSetMouseWheelCallback(&InputManager::ProcessMouseWheel);
#ifdef _GLFW_DMITRI_WINDOWS_TOUCH_ENABLED
	glfwSetTouchCallback(&InputManager::ProcessTouch);
#endif // _GLFW_DMITRI_WINDOWS_TOUCH_ENABLED
}

void InputManager::RemoveGlfwCallbacks()
{
	glfwSetWindowSizeCallback(nullptr);

	glfwSetKeyCallback(nullptr);
	glfwSetCharCallback(nullptr);
	glfwSetMouseButtonCallback(nullptr);
	glfwSetMousePosCallback(nullptr);
	glfwSetMouseWheelCallback(nullptr);
#ifdef _GLFW_DMITRI_WINDOWS_TOUCH_ENABLED
	glfwSetTouchCallback(nullptr);
#endif // _GLFW_DMITRI_WINDOWS_TOUCH_ENABLED

	m_TypingPointer.reset();
	m_MousePointer.reset();
}

// Process a window resize event
void GLFWCALL InputManager::ProcessWindowSize(int WindowWidth, int WindowHeight)
{
	// Set the viewport size
	glViewport(0, 0, WindowWidth, WindowHeight);

	// Update the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WindowWidth, WindowHeight, 0);
	glMatrixMode(GL_MODELVIEW);

	m_pInstance->m_WindowDimensions = Vector2n(WindowWidth, WindowHeight);
	printf("Window Dimensions: %dx%d\n", m_pInstance->m_WindowDimensions.X(), m_pInstance->m_WindowDimensions.Y());

	if (nullptr != m_pInstance->m_InputHandler)
	{
		InputEvent InputEvent;
		InputEvent.m_EventTypes.insert(InputEvent::EventType::PARENT_SIZE);		// TODO: Maybe put the event details inside?

		m_pInstance->m_InputHandler->ProcessEvent(InputEvent);
	}
}

void GLFWCALL InputManager::ProcessKey(int Key, int Action)
{
	// Do not ignore button events where the button state doesn't change, because they're needed for repeated arrow keys, backspace, enter, etc.
	//if ((GLFW_PRESS == Action) != m_pInstance->m_TypingPointer->GetPointerState().GetButtonState(Key))		// Ignore button events where the button state doesn't change
	{
		InputEvent InputEvent;
		InputEvent.m_EventTypes.insert(InputEvent::EventType::BUTTON_EVENT);
		InputEvent.m_DeviceId = 0;
		InputEvent.m_InputId = Key;
		InputEvent.m_Buttons.push_back(GLFW_PRESS == Action);
		InputEvent.m_Pointer = m_pInstance->m_TypingPointer.get();

		InputEvent.m_Pointer->ProcessEvent(InputEvent);
	}
}

void GLFWCALL InputManager::ProcessChar(int Character, int Action)
{
	if (GLFW_PRESS == Action)
	{
		InputEvent InputEvent;
		InputEvent.m_EventTypes.insert(InputEvent::EventType::CHARACTER_EVENT);
		InputEvent.m_DeviceId = 0;
		InputEvent.m_InputId = Character;
		InputEvent.m_Pointer = m_pInstance->m_TypingPointer.get();

		InputEvent.m_Pointer->ProcessEvent(InputEvent);
	}
}

void GLFWCALL InputManager::ProcessMouseButton(int MouseButton, int Action)
{
	{
		InputEvent InputEvent;
		InputEvent.m_EventTypes.insert(InputEvent::EventType::BUTTON_EVENT);
		InputEvent.m_DeviceId = 0;
		InputEvent.m_InputId = MouseButton;
		InputEvent.m_Buttons.push_back(GLFW_PRESS == Action);
		InputEvent.m_Pointer = m_pInstance->m_MousePointer.get();

		InputEvent.m_Pointer->ProcessEvent(InputEvent);
	}
}

void GLFWCALL InputManager::ProcessMousePos(int MousePositionX, int MousePositionY)
{
//printf("InputManager::ProcessMousePos %d %d\n", MousePositionX, MousePositionY);
	if (m_pInstance->m_MouseIgnorePositionAlways)
	{
		//printf("mm - %d,%d (Always)\n", MouseMovedX, MouseMovedY);
		return;
	}

	if (m_pInstance->m_MouseIgnorePositionOnce)
	{
		//printf("Mouse NOT moved by %d, %d to new pos %d, %d\n", MouseMovedX, MouseMovedY, MousePositionX, MousePositionY);
		m_pInstance->m_MouseIgnorePositionOnce = false;
		return;
	}

	if (true == m_pInstance->IsMouseCursorVisible())
	{
		{
			InputEvent InputEvent;
			InputEvent.m_EventTypes.insert(InputEvent::EventType::AXIS_EVENT);
			InputEvent.m_DeviceId = 0;
			InputEvent.m_InputId = 0;
			//InputEvent.m_Sliders.push_back(static_cast<double>(MouseMovedX));
			//InputEvent.m_Sliders.push_back(static_cast<double>(MouseMovedY));
			InputEvent.m_Axes.push_back(Input::AxisState(MousePositionX, m_pInstance->GetWindowDimensions()[0]));
			InputEvent.m_Axes.push_back(Input::AxisState(MousePositionY, m_pInstance->GetWindowDimensions()[1]));
			InputEvent.m_Pointer = m_pInstance->m_MousePointer.get();

			InputEvent.m_Pointer->ProcessEvent(InputEvent);
		}
	}

	// TODO: Deal with invisible mouse cursor...
	// TODO: Should I even treat it as a pointer device? Maybe not.
}

void GLFWCALL InputManager::ProcessMouseWheel(int MouseWheelPosition, int MouseWheel2Position)
{
	{
		InputEvent InputEvent;
		InputEvent.m_EventTypes.insert(InputEvent::EventType::AXIS_EVENT);
		InputEvent.m_DeviceId = 0;
		InputEvent.m_InputId = 2;
		InputEvent.m_Axes.push_back(Input::AxisState(MouseWheelPosition, 0));
		InputEvent.m_Axes.push_back(Input::AxisState(MouseWheel2Position, 1));
		InputEvent.m_Pointer = m_pInstance->m_MousePointer.get();

		InputEvent.m_Pointer->ProcessEvent(InputEvent);
	}
//printf("InputManager::ProcessMouseWheel(int MouseWheelPosition = %d), moved = %d\n", MouseWheelPosition, MouseWheelMoved);
}

#ifdef _GLFW_DMITRI_WINDOWS_TOUCH_ENABLED
/*void GLFWCALL InputManager::ProcessTouchButton(int TouchButton, int Action)
{
	{
		//m_pInstance->m_TouchPointers[TouchButton]->ProcessButton(0, (GLFW_PRESS == Action));
		InputEvent InputEvent;
		InputEvent.m_EventType = -1;
		InputEvent.m_DeviceId = 2000;		// Touch 0
		InputEvent.m_InputId = TouchButton;
		InputEvent.m_Buttons.push_back(GLFW_PRESS == Action);
		InputEvent.m_Pointer = m_pInstance->m_TouchPointers[TouchButton].get();

		InputEvent.m_Pointer->ProcessEvent(InputEvent);
	}

	for (std::vector<PointerInputListener *>::reverse_iterator it0 = m_pInstance->m_PointerListeners.rbegin(); it0 != m_pInstance->m_PointerListeners.rend(); ++it0)
	{
		(*it0)->ProcessButton(m_pInstance->m_TouchPointers[TouchButton].get(), Input::InputId(TouchButton), (GLFW_PRESS == Action));
	}

	if (GLFW_RELEASE == Action)
	{
		m_pInstance->m_TouchPointers.erase(m_pInstance->m_TouchPointers.find(TouchButton));
	}
printf("ProcessTouchButton(TouchButton %d, Action %d)\n", TouchButton, Action);
}*/

void GLFWCALL InputManager::ProcessTouch(int TouchButton, int Action, int TouchPositionX, int TouchPositionY)
{
	if (m_pInstance->m_TouchPointers.end() == m_pInstance->m_TouchPointers.find(TouchButton))
	{
		m_pInstance->m_TouchPointers.insert(std::pair<uint8, std::unique_ptr<TouchPointer>>(TouchButton, std::unique_ptr<TouchPointer>(new TouchPointer(*m_pInstance->m_InputHandler))));
	}

	{
		InputEvent InputEvent;
		//InputEvent.m_EventTypes.insert(InputEvent::EventType::SLIDER_EVENT);
		InputEvent.m_EventTypes.insert(InputEvent::EventType::AXIS_EVENT);
		InputEvent.m_DeviceId = 0;
		InputEvent.m_InputId = 0;		// TODO: Consider adding an m_PointerId? Maybe? Maybe not needed since m_Pointer uniquely identifies the pointer, so pointer ID might be a bad idea.
		//InputEvent.m_Sliders.push_back(TouchMovedX);
		//InputEvent.m_Sliders.push_back(TouchMovedY);
		InputEvent.m_Axes.push_back(Input::AxisState(TouchPositionX, m_pInstance->GetWindowDimensions()[0]));
		InputEvent.m_Axes.push_back(Input::AxisState(TouchPositionY, m_pInstance->GetWindowDimensions()[1]));
		InputEvent.m_Pointer = m_pInstance->m_TouchPointers[TouchButton].get();

		if (0 == Action)
		{
			InputEvent.m_EventTypes.insert(InputEvent::EventType::BUTTON_EVENT);
			InputEvent.m_EventTypes.insert(InputEvent::EventType::POINTER_ACTIVATION);
			InputEvent.m_Buttons.push_back(true);
		}
		else if (2 == Action)
		{
			InputEvent.m_EventTypes.insert(InputEvent::EventType::BUTTON_EVENT);
			InputEvent.m_EventTypes.insert(InputEvent::EventType::POINTER_DEACTIVATION);
			InputEvent.m_Buttons.push_back(false);
		}

		InputEvent.m_Pointer->ProcessEvent(InputEvent);
	}

	if (2 == Action)
	{
		m_pInstance->m_TouchPointers.erase(m_pInstance->m_TouchPointers.find(TouchButton));
	}

#if 0
	if (1 == Action)
	{
		{
			//m_pInstance->m_TouchPointers[TouchButton]->ProcessSlider(0 /* Touch X Axis */, static_cast<double>(TouchMovedX));
			//m_pInstance->m_TouchPointers[TouchButton]->ProcessSlider(1 /* Touch Y Axis */, static_cast<double>(TouchMovedY));
			InputEvent InputEvent;
			InputEvent.m_EventType = -1;
			InputEvent.m_DeviceCategory = 2;
			InputEvent.m_DeviceId = 0;
			InputEvent.m_InputId = TouchButton;
			InputEvent.m_Sliders.push_back(static_cast<double>(TouchMovedX));
			InputEvent.m_Sliders.push_back(static_cast<double>(TouchMovedY));
			if (true == m_pInstance->IsMouseCursorVisible())
			{
				InputEvent.m_Axes.push_back(Input::AxisState(static_cast<double>(TouchPosition.X()), -1));
				InputEvent.m_Axes.push_back(Input::AxisState(static_cast<double>(TouchPosition.Y()), -1));
			}
			InputEvent.m_Pointer = m_pInstance->m_TouchPointers[TouchButton].get();
			// TODO: Add button down component?

			InputEvent.m_Pointer->ProcessEvent(InputEvent);
		}
	}
	else if (0 == Action)
	{
		{
			InputEvent InputEvent;
			InputEvent.m_EventType = -1;
			InputEvent.m_DeviceId = 2000;		// Touch 0
			InputEvent.m_InputId = TouchButton;
			InputEvent.m_Axes.push_back(Input::AxisState(static_cast<double>(TouchPosition.X()), -1));
			InputEvent.m_Axes.push_back(Input::AxisState(static_cast<double>(TouchPosition.Y()), -1));
			InputEvent.m_Pointer = m_pInstance->m_TouchPointers[TouchButton].get();

			InputEvent.m_Pointer->ProcessEvent(InputEvent);
		}
	}
#endif

//printf("ProcessTouch(TouchButton %d, Action %d, X %d, Y %d)\n", TouchButton, Action, TouchPositionX, TouchPositionY);
}
#endif // _GLFW_DMITRI_WINDOWS_TOUCH_ENABLED

void InputManager::ProcessTimePassed(const double TimePassed)
{
	m_TypingPointer->ProcessTimePassed(TimePassed);
	m_MousePointer->ProcessTimePassed(TimePassed);

	for (auto & Pointer : m_TouchPointers)
	{
		Pointer.second->ProcessTimePassed(TimePassed);
	}

	m_InputHandler->ProcessTimePassed(TimePassed);
}
