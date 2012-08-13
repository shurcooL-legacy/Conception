#include "../Main.h"

// TODO: I've made this into a multi-line edit box, so change class name from Field (i.e. 1 line) to Box
TextFieldWidget::TextFieldWidget(Vector2n Position)
	: Widget(Position, Vector2n(904, (3 + 2/*f.body_lines.size()*/) * lineHeight)),
	  m_Content(),
	  m_ContentLines(),
	  m_MaxLineLength(0),
	  m_CaretPosition(0),
	  m_TargetCaretColumnX(0),
	  m_SelectionPosition(0)
{
	ModifyGestureRecognizer().m_RecognizeTap = true;
	ModifyGestureRecognizer().m_RecognizeManipulationTranslate = true;
	
	// DEBUG: For testing purposes
	m_Content = "int main(int argc, char * argv[])\n{\n\tPrintHi();\n\treturn 0;\n}";
	UpdateContentLines();
}

TextFieldWidget::~TextFieldWidget()
{
}

void TextFieldWidget::Render()
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
	OpenGLStream << m_Content.substr(0, std::min(m_CaretPosition, m_SelectionPosition));

	Vector2n CaretPosition;

	// Remember caret position at selection front
	if (std::min(m_CaretPosition, m_SelectionPosition) == m_CaretPosition)
	{
		CaretPosition = OpenGLStream.GetCaretPosition();
	}

	//if (CheckHover())
	// HACK
	if (ModifyGestureRecognizer().GetConnected().end() != ModifyGestureRecognizer().GetConnected().find(g_InputManager->m_TypingPointer.get()))
	{
		OpenGLStream.SetBackgroundColor(Color(195 / 255.0, 212 / 255.0, 242 / 255.0));
	}
	else
	{
		OpenGLStream.SetBackgroundColor(Color(212 / 255.0, 212 / 255.0, 212 / 255.0));
	}
	auto SelectionLength = std::max(m_CaretPosition, m_SelectionPosition) - std::min(m_CaretPosition, m_SelectionPosition);
	OpenGLStream << m_Content.substr(std::min(m_CaretPosition, m_SelectionPosition), SelectionLength);
	OpenGLStream.SetBackgroundColor(Color(1, 1, 1));

	// Remember caret position at selection back
	if (std::max(m_CaretPosition, m_SelectionPosition) == m_CaretPosition)
	{
		CaretPosition = OpenGLStream.GetCaretPosition();
	}

	OpenGLStream << m_Content.substr(std::max(m_CaretPosition, m_SelectionPosition));

	//if (CheckHover())
	// HACK
	if (ModifyGestureRecognizer().GetConnected().end() != ModifyGestureRecognizer().GetConnected().find(g_InputManager->m_TypingPointer.get()))
	{
		// Draw caret
		//if (static_cast<int>(glfwGetTime() * 2) % 2)
		if (true)
		{
			glPushMatrix();
			glTranslated(CaretPosition.X(), CaretPosition.Y(), 0);
			glColor3d(0, 0, 0);
			glBegin(GL_QUADS);
				glVertex2d(-1, 0);
				glVertex2d(-1, lineHeight);
				glVertex2d(+1, lineHeight);
				glVertex2d(+1, 0);
			glEnd();
			glPopMatrix();
		}
	}
}

void TextFieldWidget::ProcessTap()
{
	g_InputManager->RequestTypingPointer(ModifyGestureRecognizer());
}

void TextFieldWidget::ProcessCharacter(InputEvent & InputEvent, const uint32 Character)
{
	if (Character < 128u)
	{
		EraseSelectionIfAny();

		m_Content.insert(m_CaretPosition, 1, static_cast<uint8>(Character));
		MoveCaret(+1, true);

		InputEvent.m_Handled = true;
	}
}

void TextFieldWidget::ProcessManipulationStarted(const PointerState & PointerState)
{
	if (ModifyGestureRecognizer().GetConnected().end() == ModifyGestureRecognizer().GetConnected().find(g_InputManager->m_TypingPointer.get()))
	{
		auto ParentLocalPosition = GlobalToParent(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));

		ModifyGestureRecognizer().m_ManipulationOffset = GetPosition() - ParentLocalPosition;
	}
}

void TextFieldWidget::ProcessManipulationUpdated(const PointerState & PointerState)
{
	if (ModifyGestureRecognizer().GetConnected().end() == ModifyGestureRecognizer().GetConnected().find(g_InputManager->m_TypingPointer.get()))
	{
		auto ParentLocalPosition = GlobalToParent(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));

		ModifyPosition() = GetGestureRecognizer().m_ManipulationOffset + ParentLocalPosition;
	}
}

void TextFieldWidget::ProcessManipulationCompleted(const PointerState & PointerState)
{
}

void TextFieldWidget::ProcessEvent(InputEvent & InputEvent)
{
	// DECISION
	//if (CheckHover())
	// HACK
	//if (ModifyGestureRecognizer().GetConnected().end() != ModifyGestureRecognizer().GetConnected().find(g_InputManager->m_TypingPointer.get()))
	{
		// TEST
		if (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::POINTER_ACTIVATION)
			&& (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::BUTTON_EVENT)
				&& 0 == InputEvent.m_InputId
				&& true == InputEvent.m_Buttons[0]))
		{
			InputEvent.m_Pointer->ModifyPointerMapping().RequestPointerCapture(&ModifyGestureRecognizer());
		}
	}

	// DECISION
	// TEST
	// If captured by something else, ignore this event
	if (   nullptr != InputEvent.m_Pointer->GetPointerMapping().GetCapturer()
		&& &GetGestureRecognizer() != InputEvent.m_Pointer->GetPointerMapping().GetCapturer())
	{
		return;
	}

	auto SelectionLength = std::max(m_CaretPosition, m_SelectionPosition) - std::min(m_CaretPosition, m_SelectionPosition);

	if (InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::BUTTON_EVENT))
	{
		if (Pointer::VirtualCategory::TYPING == InputEvent.m_Pointer->GetVirtualCategory())
		{
			auto ButtonId = InputEvent.m_InputId;
			bool Pressed = InputEvent.m_Buttons[0];		// TODO: Check if there are >1 buttons

			if (Pressed)
			{
				auto ShiftActive = (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LSHIFT)
									|| InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RSHIFT));
				auto SuperActive = (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LSUPER)
									|| InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RSUPER));
				auto AltActive = (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LALT)
								  || InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RALT));

				switch (ButtonId)
				{
				case GLFW_KEY_BACKSPACE:
					{
						auto SelectionExisted = EraseSelectionIfAny();

						if (false == SelectionExisted)
						{
							if (m_CaretPosition > 0)
							{
								m_Content.erase(m_CaretPosition - 1, 1);
								MoveCaret(-1, true);
							}
						}
					}
					break;
				case GLFW_KEY_DEL:
					{
						auto SelectionExisted = EraseSelectionIfAny();

						if (false == SelectionExisted)
						{
							if (m_CaretPosition < m_Content.length())
							{
								m_Content.erase(m_CaretPosition, 1);
							}
						}
					}
					break;
				case GLFW_KEY_ENTER:
				case GLFW_KEY_KP_ENTER:
					{
						EraseSelectionIfAny();

						m_Content.insert(m_CaretPosition, 1, '\n');
						MoveCaret(+1, true);
					}
					break;
				case GLFW_KEY_TAB:
					{
						EraseSelectionIfAny();

						m_Content.insert(m_CaretPosition, 1, '\t');
						MoveCaret(+1, true);
					}
					break;
				case GLFW_KEY_LEFT:
					{
						if (0 != SelectionLength && !ShiftActive)
						{
							SetCaretPosition(std::min(m_CaretPosition, m_SelectionPosition), true);
						}
						else
						{
							if (SuperActive && !AltActive)
							{
								UpdateContentLines();

								std::vector<class ContentLine>::size_type LineNumber = 0;
								std::vector<class ContentLine>::size_type ColumnNumber = 0;

								for (auto & ContentLine : m_ContentLines)
								{
									if (ContentLine.m_StartPosition + ContentLine.m_Length >= m_CaretPosition)
									{
										ColumnNumber = m_CaretPosition - ContentLine.m_StartPosition;
										break;
									}

									++LineNumber;
								}

								SetCaretPosition(m_ContentLines[LineNumber].m_StartPosition, !ShiftActive);
							}
							else if (AltActive && !SuperActive)
							{
								{
									// Skip spaces to the left
									auto LookAt = m_CaretPosition - 1;
									while (   LookAt != -1
										   && (' ' == m_Content[LookAt] || '\n' == m_Content[LookAt] || '\t' == m_Content[LookAt]))
									{
										--LookAt;
									}

									// Skip non-spaces to the left
									while (   LookAt != -1
										   && !(' ' == m_Content[LookAt] || '\n' == m_Content[LookAt] || '\t' == m_Content[LookAt]))
									{
										--LookAt;
									}

									SetCaretPosition(LookAt + 1, !ShiftActive);
								}
							}
							else
							{
								MoveCaretTry(-1, !ShiftActive);
							}
						}
					}
					break;
				case GLFW_KEY_RIGHT:
					{
						if (0 != SelectionLength && !ShiftActive)
						{
							SetCaretPosition(std::max(m_CaretPosition, m_SelectionPosition), true);
						}
						else
						{
							if (SuperActive && !AltActive)
							{
								UpdateContentLines();

								std::vector<class ContentLine>::size_type LineNumber = 0;
								std::vector<class ContentLine>::size_type ColumnNumber = 0;

								for (auto & ContentLine : m_ContentLines)
								{
									if (ContentLine.m_StartPosition + ContentLine.m_Length >= m_CaretPosition)
									{
										ColumnNumber = m_CaretPosition - ContentLine.m_StartPosition;
										break;
									}

									++LineNumber;
								}

								SetCaretPosition(m_ContentLines[LineNumber].m_StartPosition + m_ContentLines[LineNumber].m_Length, !ShiftActive);
							}
							else if (AltActive && !SuperActive)
							{
								{
									// Skip spaces to the right
									auto LookAt = m_CaretPosition;
									while (   LookAt < m_Content.length()
										   && (' ' == m_Content[LookAt] || '\n' == m_Content[LookAt] || '\t' == m_Content[LookAt]))
									{
										++LookAt;
									}

									// Skip non-spaces to the right
									while (   LookAt < m_Content.length()
										   && !(' ' == m_Content[LookAt] || '\n' == m_Content[LookAt] || '\t' == m_Content[LookAt]))
									{
										++LookAt;
									}

									SetCaretPosition(LookAt, !ShiftActive);
								}
							}
							else
							{
								MoveCaretTry(+1, !ShiftActive);
							}
						}
					}
					break;
				case GLFW_KEY_UP:
					{
						if (0 != SelectionLength && !ShiftActive)
						{
							SetCaretPosition(std::min(m_CaretPosition, m_SelectionPosition), true);
						}

						MoveCaretVerticallyTry(-1, !ShiftActive);
					}
					break;
				case GLFW_KEY_DOWN:
					{
						if (0 != SelectionLength && !ShiftActive)
						{
							SetCaretPosition(std::max(m_CaretPosition, m_SelectionPosition), true);
						}

						MoveCaretVerticallyTry(+1, !ShiftActive);
					}
					break;
				case 'A':
					{
						if (SuperActive)
						{
							// Select all
							SetCaretPosition(0, true);
							SetCaretPosition(m_Content.length(), false);
						}
					}
					break;
				case 'X':
					{
						if (SuperActive)
						{
							glfwSetClipboardString(GetSelectionContent());

							EraseSelectionIfAny();
						}
					}
					break;
				case 'C':
					{
						if (SuperActive)
						{
							glfwSetClipboardString(GetSelectionContent());
						}
					}
					break;
				case 'V':
					{
						if (SuperActive)
						{
							EraseSelectionIfAny();

							m_Content.insert(m_CaretPosition, glfwGetClipboardString());
							MoveCaret(static_cast<sint32>(glfwGetClipboardString().length()), true);
						}
					}
					break;
				default:
					break;
				}
			}
		}
		else if (Pointer::VirtualCategory::POINTING == InputEvent.m_Pointer->GetVirtualCategory())
		{
			auto ButtonId = InputEvent.m_InputId;
			bool Pressed = InputEvent.m_Buttons[0];		// TODO: Check if there are >1 buttons

			if (Pressed)
			{
				switch (ButtonId)
				{
				case 0:
					{
						Vector2n GlobalPosition(InputEvent.m_Pointer->GetPointerState().GetAxisState(0).GetPosition(), InputEvent.m_Pointer->GetPointerState().GetAxisState(1).GetPosition());
						Vector2n LocalPosition = GlobalToLocal(GlobalPosition);

						auto CaretPosition = GetNearestCaretPosition(LocalPosition);

						auto ShiftActive = g_InputManager->m_TypingPointer->GetPointerState().GetButtonState(GLFW_KEY_LSHIFT) || g_InputManager->m_TypingPointer->GetPointerState().GetButtonState(GLFW_KEY_RSHIFT);
						SetCaretPosition(CaretPosition, !ShiftActive);
					}
					break;
				default:
					break;
				}
			}
		}
	}

	if (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::AXIS_EVENT)
		|| InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::CANVAS_MOVED_TEST))
	{
		if (Pointer::VirtualCategory::POINTING == InputEvent.m_Pointer->GetVirtualCategory())
		{
			if (true == InputEvent.m_Pointer->GetPointerState().GetButtonState(0))
			{
				Vector2n GlobalPosition(InputEvent.m_Pointer->GetPointerState().GetAxisState(0).GetPosition(), InputEvent.m_Pointer->GetPointerState().GetAxisState(1).GetPosition());
				Vector2n LocalPosition = GlobalToLocal(GlobalPosition);
				if (LocalPosition.X() < 0)
					LocalPosition.X() = 0;
				if (LocalPosition.Y() < 0)
					LocalPosition.Y() = 0;

				auto CaretPosition = GetNearestCaretPosition(LocalPosition);

				SetCaretPosition(CaretPosition, false);
			}
		}
	}
}

void TextFieldWidget::SetCaretPosition(decltype(m_CaretPosition) CaretPosition, bool ResetSelection, bool UpdateTargetCaretColumn)
{
	m_CaretPosition = CaretPosition;

	if (ResetSelection)
	{
		m_SelectionPosition = m_CaretPosition;
	}

	if (UpdateTargetCaretColumn)
	{
		UpdateContentLines();

		std::vector<class ContentLine>::size_type LineNumber = 0;
		std::vector<class ContentLine>::size_type ColumnNumber = 0;

		for (auto & ContentLine : m_ContentLines)
		{
			if (ContentLine.m_StartPosition + ContentLine.m_Length >= m_CaretPosition)
			{
				ColumnNumber = m_CaretPosition - ContentLine.m_StartPosition;
				break;
			}

			++LineNumber;
		}

		m_TargetCaretColumnX = GetCaretPositionX(LineNumber, ColumnNumber);
	}
}

void TextFieldWidget::MoveCaret(sint32 MoveAmount, bool ResetSelection)
{
	SetCaretPosition(m_CaretPosition + MoveAmount, ResetSelection);
}

void TextFieldWidget::MoveCaretTry(sint32 MoveAmount, bool ResetSelection)
{
	if (   (MoveAmount < 0 && -MoveAmount <= m_CaretPosition)
		|| (MoveAmount > 0 && m_CaretPosition + MoveAmount <= m_Content.length()))
	{
		SetCaretPosition(m_CaretPosition + MoveAmount, ResetSelection);
	}
}

void TextFieldWidget::MoveCaretVerticallyTry(sint32 MoveAmount, bool ResetSelection)
{
	UpdateContentLines();

	std::vector<class ContentLine>::size_type LineNumber = 0;
	std::vector<class ContentLine>::size_type ColumnNumber = 0;

	for (auto & ContentLine : m_ContentLines)
	{
		if (ContentLine.m_StartPosition + ContentLine.m_Length >= m_CaretPosition)
		{
			ColumnNumber = m_CaretPosition - ContentLine.m_StartPosition;
			break;
		}

		++LineNumber;
	}

	if (-1 == MoveAmount)
	{
		if (LineNumber > 0)
			--LineNumber;
	}
	else if (+1 == MoveAmount)
	{
		if (LineNumber < m_ContentLines.size() - 1)
			++LineNumber;
	}
	//m_CaretPosition = std::min(m_ContentLines[LineNumber].m_StartPosition + ColumnNumber, m_ContentLines[LineNumber].m_StartPosition + m_ContentLines[LineNumber].m_Length);
	{
		auto CaretPosition = GetNearestCaretPosition(LineNumber, m_TargetCaretColumnX);

		SetCaretPosition(CaretPosition, ResetSelection, false);
	}
}

std::string TextFieldWidget::GetSelectionContent()
{
	auto SelectionLength = std::max(m_CaretPosition, m_SelectionPosition) - std::min(m_CaretPosition, m_SelectionPosition);

	return m_Content.substr(std::min(m_CaretPosition, m_SelectionPosition), SelectionLength);
}

// Returns true if there was a selection, false otherwise
bool TextFieldWidget::EraseSelectionIfAny()
{
	auto SelectionLength = std::max(m_CaretPosition, m_SelectionPosition) - std::min(m_CaretPosition, m_SelectionPosition);

	if (0 != SelectionLength)
	{
		m_Content.erase(std::min(m_CaretPosition, m_SelectionPosition), SelectionLength);

		SetCaretPosition(std::min(m_CaretPosition, m_SelectionPosition), true);
	}

	return (0 != SelectionLength);
}

// Update Content Lines structure
void TextFieldWidget::UpdateContentLines()
{
	m_ContentLines.clear();
	m_MaxLineLength = 0;

	std::string::size_type Start = 0, End;
	do
	{
		End = m_Content.find_first_of('\n', Start);

		auto Length = ((std::string::npos != End) ? End : m_Content.length()) - Start;
		m_ContentLines.push_back(ContentLine(Start, Length));
		auto LengthX = GetCaretPositionX(m_ContentLines.size() - 1, Length) / charWidth;
		if (m_MaxLineLength < LengthX)
			m_MaxLineLength = LengthX;

		Start = End + 1;
	}
	while (std::string::npos != End);

	// TEST: Resize the widget to accomodate text width
	ModifyDimensions().X() = std::max<sint32>(static_cast<sint32>(m_MaxLineLength * charWidth), 3 * charWidth);
	ModifyDimensions().Y() = std::max<sint32>(static_cast<sint32>(m_ContentLines.size()) * lineHeight, 1 * lineHeight);
}

uint32 TextFieldWidget::GetCaretPositionX(std::vector<class ContentLine>::size_type LineNumber, std::vector<class ContentLine>::size_type ColumnNumber)
{
	uint32 CaretPositionX = 0;

	{
		std::string Line = m_Content.substr(m_ContentLines[LineNumber].m_StartPosition, ColumnNumber);

		std::string::size_type Start = 0, End;
		do
		{
			End = Line.find_first_of('\t', Start);

			auto Length = ((std::string::npos != End) ? End : Line.length()) - Start;
			//PrintSegment(Line.substr(Start, Length));
			CaretPositionX += Length * charWidth;
			if (std::string::npos != End)
			{
				//Tab();
				CaretPositionX = ((CaretPositionX / charWidth / 4) + 1) * 4 * charWidth;
			}

			Start = End + 1;
		}
		while (std::string::npos != End);
	}

	return CaretPositionX;
}

decltype(TextFieldWidget::m_CaretPosition) TextFieldWidget::GetNearestCaretPosition(Vector2n LocalPosition)
{
	uint32 LineNumber = LocalPosition.Y() / lineHeight;

	return GetNearestCaretPosition(LineNumber, static_cast<uint32>(LocalPosition.X()));
}

decltype(TextFieldWidget::m_CaretPosition) TextFieldWidget::GetNearestCaretPosition(std::vector<class ContentLine>::size_type LineNumber, uint32 LocalPositionX)
{
	UpdateContentLines();

	// Calculate nearest caret position
	if (LineNumber > m_ContentLines.size() - 1)
		LineNumber = m_ContentLines.size() - 1;
	std::string::size_type CharacterNumber = (LocalPositionX + charWidth / 2) / charWidth;
	if (CharacterNumber > m_ContentLines[LineNumber].m_Length)
		CharacterNumber = m_ContentLines[LineNumber].m_Length;

	{
		std::string Line = m_Content.substr(m_ContentLines[LineNumber].m_StartPosition, m_ContentLines[LineNumber].m_Length);

		uint32 CaretPositionX = 0;
		std::string::size_type Start = 0, End;
		do
		{
			End = Line.find_first_of('\t', Start);

			auto Length = ((std::string::npos != End) ? End : Line.length()) - Start;
			//PrintSegment(Line.substr(Start, Length));
			CaretPositionX += Length * charWidth;
			if (CaretPositionX >= LocalPositionX)
			{
				CharacterNumber = Start + (LocalPositionX - (CaretPositionX - Length * charWidth) + charWidth / 2) / charWidth;
				break;
			}
			if (std::string::npos != End)
			{
				//Tab();
				auto StartCaretPositionX = CaretPositionX;
				CaretPositionX = ((CaretPositionX / charWidth / 4) + 1) * 4 * charWidth;
				if (CaretPositionX >= LocalPositionX)
				{
					if (CaretPositionX - LocalPositionX > LocalPositionX - StartCaretPositionX)
						CharacterNumber = Start + Length;
					else
						CharacterNumber = Start + Length + 1;
					break;
				}
			}

			Start = End + 1;
		}
		while (std::string::npos != End);
	}

	return (m_ContentLines[LineNumber].m_StartPosition + CharacterNumber);
}
