#pragma once
#ifndef __InputManager_H__
#define __InputManager_H__

class InputManager
{
public:
	InputManager();
	~InputManager();

	void SetInputHandler(InputHandler * InputHandler);
	//ConceptionApp * GetAppTEST() { return reinterpret_cast<ConceptionApp *>(m_InputHandler); }		// Test function to allow global access to app... do this properly
	void ClearInputHandler();

	void SetMouseCursorVisibility(bool Visible);
	bool IsMouseCursorVisible();

	Vector2n GetWindowDimensions();

	void RequestTypingPointer(GestureRecognizer & Target);

	static void GLFWCALL ProcessWindowSize(int WindowWidth, int WindowHeight);

	static void GLFWCALL ProcessKey(int Key, int Action);
	static void GLFWCALL ProcessChar(int Character, int Action);
	static void GLFWCALL ProcessMouseButton(int MouseButton, int Action);
	static void GLFWCALL ProcessMousePos(int MousePositionX, int MousePositionY);
	static void GLFWCALL ProcessMouseWheel(int MouseWheelPosition);
#ifdef _GLFW_DMITRI_WINDOWS_TOUCH_ENABLED
	static void GLFWCALL ProcessTouch(int TouchButton, int Action, int TouchPositionX, int TouchPositionY);		// Action: 0 = Down, 1 = Move, 2 = Up
#endif // _GLFW_DMITRI_WINDOWS_TOUCH_ENABLED

	void ProcessTimePassed(const double TimePassed);

	struct InputId {
		uint16 Device;
		uint16 Id;

		InputId(uint16 Device, uint16 Id) {
			this->Device = Device;
			this->Id = Id;
		}

		bool operator < (const InputId & Other) const {
			uint32 ThisValue = (static_cast<uint32>(this->Device) << 16) | (this->Id);
			uint32 OtherValue = (static_cast<uint32>(Other.Device) << 16) | (Other.Id);
			return ThisValue < OtherValue;
		}
	};

	typedef uint16 VirtualInputId;

private:
	InputManager(const InputManager &);
	InputManager & operator =(const InputManager &);

	void SetGlfwCallbacks();
	void RemoveGlfwCallbacks();

	public:std::unique_ptr<TypingPointer>						m_TypingPointer;private:	// HACK
	public:std::unique_ptr<MousePointer>						m_MousePointer;private:		// HACK
	std::map<uint8, std::unique_ptr<TouchPointer>>		m_TouchPointers;

	InputHandler *	m_InputHandler;		// MAYBE: Change the type to App?

	volatile bool		m_MouseCursorVisible;
	volatile bool		m_MouseIgnorePositionAlways;
	volatile bool		m_MouseIgnorePositionOnce;

	Vector2n			m_WindowDimensions;

	static InputManager *		m_pInstance;
};

#endif // __InputManager_H__
