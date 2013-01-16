#include "../Main.h"

ConceptStringBoxWidget::ConceptStringBoxWidget(Vector2n Position, TypingModule & TypingModule)
	: Widget(Position, Vector2n(80 * charWidth, 24 * lineHeight), {}),
	  m_Content(),
	  m_CaretPosition(0),
	  m_TypingModule(TypingModule)
{
	SetupGestureRecognizer();

	// DEBUG: Irregular starting state, for testing
	{
		/*m_Content.push_back(FindConcept("int"));
		m_Content.push_back(FindConcept("main"));
		m_Content.push_back(FindConcept("("));
		m_Content.push_back(FindConcept("int"));
		m_Content.push_back(FindConcept("argc"));
		m_Content.push_back(FindConcept(","));
		m_Content.push_back(FindConcept("char"));
		m_Content.push_back(FindConcept("*"));
		m_Content.push_back(FindConcept("argv"));
		m_Content.push_back(FindConcept(")"));
		m_Content.push_back(FindConcept("{"));
		m_Content.push_back(17);
		m_Content.push_back(FindConcept("("));
		m_Content.push_back(FindConcept(")"));
		m_Content.push_back(FindConcept(";"));
		m_Content.push_back(FindConcept("return"));
		m_Content.push_back(FindConcept("0"));
		m_Content.push_back(FindConcept(";"));
		m_Content.push_back(FindConcept("}"));*/

		/*
		// Skip non-spaces to the right
		auto LookAt = m_CaretPosition;
		while (   LookAt < m_Content.length()
			   && IsCoreCharacter(m_Content[LookAt]))
		{
			++LookAt;
		}
		
		SetCaretPosition(LookAt, false);
		*/

		/*m_Content.push_back(FindOrCreateConcept("// Skip non-spaces to the right"));
		m_Content.push_back(FindOrCreateConcept("auto LookAt = m_CaretPosition;"));
		m_Content.push_back(FindOrCreateConcept("while ("));
		m_Content.push_back(FindOrCreateConcept("   LookAt < m_Content.length()\n&& IsCoreCharacter(m_Content[LookAt])"));
		m_Content.push_back(FindOrCreateConcept(")"));
		m_Content.push_back(FindOrCreateConcept("{"));
		m_Content.push_back(FindOrCreateConcept("++LookAt;"));
		m_Content.push_back(FindOrCreateConcept("}"));
		m_Content.push_back(FindOrCreateConcept(""));
		m_Content.push_back(FindOrCreateConcept("SetCaretPosition(LookAt, false);"));*/

		//m_Content.push_back(37);
		/*m_Content.push_back(38);
		m_Content.push_back(ConceptInstance(38, {33}));
		m_Content.push_back(ConceptInstance(39, {33}));*/
		//m_Content.push_back(ConceptInstance(40, {36, 28}));
		m_Content.push_back(LastConceptId() - 2);
	}
}

ConceptStringBoxWidget::~ConceptStringBoxWidget()
{
}

void ConceptStringBoxWidget::SetupGestureRecognizer()
{
	//ModifyGestureRecognizer().m_RecognizeTap = true;

	// HACK: Recognize only taps when unselected; but this needs to be automated
	ModifyGestureRecognizer().m_RecognizeTap = !HasTypingFocus();
}

void ConceptStringBoxWidget::ProcessTimePassed(const double TimePassed)
{
	SetupGestureRecognizer();
}

void ConceptStringBoxWidget::Render()
{
	Color BackgroundColor(1.0, 1.0, 1.0);
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
	if (HasTypingFocus())
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
	//OpenGLStream << m_Content.substr(0, m_CaretPosition);
	// TODO: Optimize this somehow?
	OpenGLStream << decltype(m_Content)(m_Content.begin(), m_Content.begin() + m_CaretPosition);

	// TEST
	if (!m_TypingModule.GetString().empty())
	{
		for (auto & Pointer : GetGestureRecognizer().GetConnected())
		{
			if (Pointer::VirtualCategory::POINTING == Pointer->GetVirtualCategory())
			{
				//Vector2n GlobalPosition(Pointer->GetPointerState().GetAxisState(0).GetPosition(), Pointer->GetPointerState().GetAxisState(1).GetPosition());
				//Vector2n LocalPosition(GlobalToLocal(GlobalPosition));

				//auto ConceptId = FindOrCreateConcept(Entry);

				OpenGLStream << m_TypingModule.GetString();
			}
		}
	}

	Vector2n CaretPosition = OpenGLStream.GetCaretPosition();

	//OpenGLStream << m_Content.substr(m_CaretPosition);
	// TODO: Optimize this somehow?
	OpenGLStream << decltype(m_Content)(m_Content.begin() + m_CaretPosition, m_Content.end());

	//if (CheckHover())
	// HACK
	if (HasTypingFocus())
	{
		// Draw caret
		//if (static_cast<int>(glfwGetTime() * 2) % 2)
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

void ConceptStringBoxWidget::ProcessTap(const InputEvent & InputEvent, Vector2n Position)
{
	g_InputManager->RequestTypingPointer(ModifyGestureRecognizer());
}

void ConceptStringBoxWidget::ProcessCharacter(InputEvent & InputEvent, const uint32 Character)
{
	// TEST
	/*if (Character < 128u)
	{
		if (';' == Character)
			m_Content.push_back(11);
		else
			m_Content.push_back(17);

		InputEvent.m_Handled = true;
	}*/
}

void ConceptStringBoxWidget::ProcessEvent(InputEvent & InputEvent)
{
	if (InputEvent.HasType(InputEvent::EventType::BUTTON_EVENT))
	{
		auto ButtonId = InputEvent.m_InputId;
		bool Pressed = InputEvent.m_Buttons[0];		// TODO: Check if there are >1 buttons

		if (Pointer::VirtualCategory::TYPING == InputEvent.m_Pointer->GetVirtualCategory())
		{
			if (Pressed)
			{
				bool HandledEvent = true;		// Assume true at first

				switch (ButtonId)
				{
				case GLFW_KEY_BACKSPACE:
					/*{
						// Erase the last concept
						if (false == m_Content.empty())
						{
							m_Content.pop_back();
						}

						InputEvent.m_Handled = true;
					}*/
					break;
				case GLFW_KEY_ENTER:
					{
					}
					break;
				case GLFW_KEY_TAB:
					{
					}
					break;
				case GLFW_KEY_LEFT:
					{
						MoveCaretTry(-1, true);
					}
					break;
				case GLFW_KEY_RIGHT:
					{
						MoveCaretTry(+1, true);
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
		else if (Pointer::VirtualCategory::POINTING == InputEvent.m_Pointer->GetVirtualCategory())
		{
			if (HasTypingFocus())
			{
				if (Pressed)
				{
					bool HandledEvent = true;		// Assume true at first

					switch (ButtonId)
					{
					case 0:
						{
							auto Entry = m_TypingModule.TakeString();

							if (!Entry.empty())
							{
								auto ConceptId = FindOrCreateConcept(Entry);

								m_Content.push_back(ConceptId);
							}
							else
							{
								if (!m_Content.empty())
								{
									if (m_CaretPosition >= m_Content.size())
									{
										MoveCaretTry(-1, true);
									}

									m_TypingModule.SetString(GetConcept(m_Content.back()).GetContent());
									m_Content.pop_back();
								}
							}
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
}

void ConceptStringBoxWidget::SetCaretPosition(decltype(m_CaretPosition) CaretPosition, bool ResetSelection, bool UpdateTargetCaretColumn)
{
	m_CaretPosition = CaretPosition;
}

void ConceptStringBoxWidget::MoveCaretTry(sint32 MoveAmount, bool ResetSelection)
{
	if (   (MoveAmount < 0 && -MoveAmount <= m_CaretPosition)
		|| (MoveAmount > 0 && m_CaretPosition + MoveAmount <= m_Content.size()))
	{
		SetCaretPosition(m_CaretPosition + MoveAmount, ResetSelection);
	}
}
