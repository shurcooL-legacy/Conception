#include "../Main.h"

TypingModule::TypingModule()
	: Widget(Vector2n::ZERO, Vector2n::ZERO, {}),
	  m_Typed()
{
}

TypingModule::~TypingModule()
{
}

void TypingModule::Render()
{
	Render(*g_InputManager);
}

void TypingModule::Render(const InputManager & InputManager)
{
	// Draw "m_Typed" string
	if (m_Typed.length() > 0)
	{
		Vector2n PointerPosition(InputManager.m_MousePointer->GetPointerState().GetAxisState(0).GetPosition(), InputManager.m_MousePointer->GetPointerState().GetAxisState(1).GetPosition());
		DimensionsStream Stream;
		Stream << m_Typed;
		Vector2n Dimensions = Stream.GetDimensions();
		auto Position = GetInsertionPosition(PointerPosition);
		Position.Y() -= lineHeight / 2;

		DrawInnerBox(Position, Dimensions, Color(static_cast<uint8>(234), 233, 190));

		//glColor3d(0, 0, 0); OglUtilsPrint(Position.X(), Position.Y(), 0, LEFT, m_Typed.c_str());
		glColor3d(0, 0, 0);
		OpenGLStream OpenGLStream(Position);
		OpenGLStream << m_Typed;
	}
}

Vector2n TypingModule::GetInsertionPosition(Vector2n PointerPosition) const
{
	DimensionsStream Stream;
	Stream << m_Typed;
	Vector2n Dimensions = Stream.GetDimensions();

	//Vector2n InsertionOffset(-m_Typed.length() * charWidth / 2 + charWidth, lineHeight * 3 / 2);
	//Vector2n InsertionOffset(-m_Typed.length() * charWidth / 2, 0);
	Vector2n InsertionOffset(Dimensions.X() / -2, Dimensions.Y() / -2 + lineHeight / 2);
	//Vector2n InsertionOffset(0, 0);

	return PointerPosition + InsertionOffset;
}

/* THIS IS pointless because this has stuff to do with pointing pointers only, not typing pointer... :/
bool TypingModule::IsHit(const Vector2n ParentPosition) const
{
	return true;
}*/

void TypingModule::ProcessEvent(InputEvent & InputEvent)
{
	if (InputEvent.HasType(InputEvent::EventType::BUTTON_EVENT))
	{
		if (Pointer::VirtualCategory::TYPING == InputEvent.m_Pointer->GetVirtualCategory())
		{
			auto ButtonId = InputEvent.m_InputId;
			bool Pressed = InputEvent.m_Buttons[0];		// TODO: Check if there are >1 buttons

			if (Pressed)
			{
				const auto ControlActive = (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LCTRL)
											|| InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RCTRL));
				const auto ShiftActive = (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LSHIFT)
										  || InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RSHIFT));
				const auto SuperActive = (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LSUPER)
										  || InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RSUPER));
				const auto AltActive = (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LALT)
										|| InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RALT));

				bool HandledEvent = true;		// Assume true at first

				switch (ButtonId)
				{
				case GLFW_KEY_BACKSPACE:
					{
						if (!m_Typed.empty())
						{
							// Erase the last character in string
							m_Typed.erase(m_Typed.end() - 1);
						}
						else
							HandledEvent = false;
					}
					break;
				case GLFW_KEY_DEL:
				case GLFW_KEY_ESC:
					{
						if (!m_Typed.empty())
						{
							// Erase the entire string
							m_Typed.clear();
						}
						else
							HandledEvent = false;
					}
					break;
				case 'X':
					{
						if (   SuperActive
							&& !GetString().empty())
						{
							glfwSetClipboardString(TakeString());
						}
						else
							HandledEvent = false;
					}
					break;
				case 'C':
					{
						if (   SuperActive
							&& !GetString().empty())
						{
							glfwSetClipboardString(GetString());
						}
						else
							HandledEvent = false;
					}
					break;
				case 'V':
					{
						if (   SuperActive
							&& !glfwGetClipboardString().empty())
						{
							m_Typed.append(glfwGetClipboardString());
						}
						else
							HandledEvent = false;
					}
					break;
				default:
					HandledEvent = false;
					break;
				}

				if (HandledEvent)
				{
					InputEvent.m_Handled = true;
				}
			}
		}
	}
}

void TypingModule::ProcessCharacter(InputEvent & InputEvent, const uint32 Character)
{
	if (Character < 128u)
	/*if (   ('a' <= Character && Character <= 'z')
		|| ('A' <= Character && Character <= 'Z')
		|| ('0' <= Character && Character <= '9')
		|| ' ' == Character
		|| '-' == Character
		|| '_' == Character
		|| '.' == Character)*/
	{
		m_Typed += static_cast<uint8>(Character);

		InputEvent.m_Handled = true;
	}
}

std::string TypingModule::GetString()
{
	return m_Typed;
}

std::string TypingModule::TakeString()
{
	auto String = m_Typed;

	m_Typed.clear();

	return String;
}

void TypingModule::SetString(std::string String)
{
	m_Typed = String;
}
