#include "../Main.h"

TypingModule::TypingModule()
	: GestureHandler(),
	  m_Typed()
{
}

TypingModule::~TypingModule()
{
}

void TypingModule::ProcessEvent(InputEvent & InputEvent)
{
	if (InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::BUTTON_EVENT))
	{
		if (Pointer::VirtualCategory::TYPING == InputEvent.m_Pointer->GetVirtualCategory())
		{
			auto ButtonId = InputEvent.m_InputId;
			bool Pressed = InputEvent.m_Buttons[0];		// TODO: Check if there are >1 buttons

			if (Pressed)
			{
				switch (ButtonId)
				{
				case GLFW_KEY_BACKSPACE:
					{
						// Erase the last character in string
						if (!m_Typed.empty())
						{
							m_Typed.erase(m_Typed.end() - 1);
						}

						InputEvent.m_Handled = true;
					}
					break;
				case GLFW_KEY_DEL:
					{
						// Erase the entire string
						m_Typed.clear();

						InputEvent.m_Handled = true;
					}
					break;
				default:
					break;
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

void TypingModule::Render(const InputManager & InputManager)
{
	// Draw "m_Typed" string
	if (m_Typed.length() > 0)
	{
		//Vector2n Position(static_cast<sint32>(InputManager.m_MousePointer->GetPointerState().GetAxisState(0).GetPosition()) - m_Typed.length() * charWidth / 2 + charWidth, static_cast<sint32>(InputManager.m_MousePointer->GetPointerState().GetAxisState(1).GetPosition()) + lineHeight);
		Vector2n Position(static_cast<sint32>(InputManager.m_MousePointer->GetPointerState().GetAxisState(0).GetPosition()) - m_Typed.length() * charWidth / 2, static_cast<sint32>(InputManager.m_MousePointer->GetPointerState().GetAxisState(1).GetPosition()) - lineHeight / 2);
		Vector2n Dimensions(static_cast<sint32>(m_Typed.length()) * charWidth, 1 * lineHeight);

		DrawAroundBox(Position, Dimensions, Color(234 / 255.0, 233 / 255.0, 190 / 255.0));

		glColor3d(0, 0, 0); OglUtilsPrint(Position.X(), Position.Y(), 0, LEFT, m_Typed.c_str());
	}
}

std::string TypingModule::GetString()
{
	return m_Typed;
}

void TypingModule::SetString(std::string String)
{
	m_Typed = String;
}

void TypingModule::Clear()
{
	m_Typed.clear();
}
