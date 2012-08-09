#include "../Main.h"

ConceptStringBoxWidget::ConceptStringBoxWidget(Vector2n Position)
	: Widget(Position, Vector2n(904, (3 + 2/*f.body_lines.size()*/) * lineHeight)),
	  m_Content()
{
	ModifyGestureRecognizer().m_RecognizeTap = true;
}

ConceptStringBoxWidget::~ConceptStringBoxWidget()
{
}

void ConceptStringBoxWidget::Render()
{
	Color BackgroundColor(1, 1, 1);
	Color BorderColor(0.3, 0.3, 0.3);

	/*if (CheckHover(WidgetManager) && CheckActive(WidgetManager))
	{
	}
	else if ((CheckHover(WidgetManager) && !CheckAnyActive(WidgetManager)) || (!CheckHover(WidgetManager) && CheckActive(WidgetManager)))
	{
		BorderColor[0] = 0.898;
		BorderColor[1] = 0.765;
		BorderColor[2] = 0.396;
	}
	else
	{
	}*/
	//if (CheckHover())
	// HACK
	if (ModifyGestureRecognizer().GetConnected().end() != ModifyGestureRecognizer().GetConnected().find(g_InputManager->m_TypingPointer.get()))
	{
		BorderColor[0] = 0.898;
		BorderColor[1] = 0.765;
		BorderColor[2] = 0.396;
	}

	/*glBegin(GL_QUADS);
		glVertex2d(m_Position.X(), m_Position.Y());
		glVertex2d(m_Position.X(), m_Position.Y() + 30);
		glVertex2d(m_Position.X() + 30, m_Position.Y() + 30);
		glVertex2d(m_Position.X() + 30, m_Position.Y());
	glEnd();*/
	DrawAroundBox(GetPosition(), GetDimensions(), BackgroundColor, BorderColor);

	glColor3d(0, 0, 0);
	OpenGLStream OpenGLStream(GetPosition());
	OpenGLStream << m_Content;

	//if (CheckHover())
	// HACK
	if (ModifyGestureRecognizer().GetConnected().end() != ModifyGestureRecognizer().GetConnected().find(g_InputManager->m_TypingPointer.get()))
	{
		if (static_cast<int>(glfwGetTime() * 2) % 2)
		{
			OpenGLStream << "_";
		}
	}
}

void ConceptStringBoxWidget::ProcessTap()
{
	g_InputManager->RequestTypingPointer(ModifyGestureRecognizer());
}

void ConceptStringBoxWidget::ProcessCharacter(const int Character)
{
	// TEST
	if (static_cast<uint32>(Character) < 128)
	{
		if (';' == Character)
			m_Content.push_back(11);
		else
			m_Content.push_back(17);
	}
}

void ConceptStringBoxWidget::ProcessEvent(InputEvent & InputEvent)
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
						// Erase the last concept
						if (false == m_Content.empty())
						{
							m_Content.pop_back();
						}
					}
					break;
				case GLFW_KEY_ENTER:
					{
					}
					break;
				case GLFW_KEY_TAB:
					{
					}
					break;
				default:
					break;
				}
			}
		}
	}
}
