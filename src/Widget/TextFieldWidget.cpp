#include "../Main.h"

// TODO: I've made this into a multi-line edit box, so change class name from Field (i.e. 1 line) to Box
TextFieldWidget::TextFieldWidget(Vector2n Position, TypingModule & TypingModule)
	: CompositeWidget(Position, {}, {}),
	  m_Content(),
	  m_CaretPosition(0),
	  m_SelectionPosition(0),
	  m_TargetCaretColumnX(0),
	  m_ContentLines(),
	  m_MaxLineLength(0),
	  m_TypingModule(TypingModule)
{
	SetupGestureRecognizer();

	UpdateContentLines();		// This is here at least for resize
}

TextFieldWidget::~TextFieldWidget()
{
}

void TextFieldWidget::SetupGestureRecognizer()
{
	//ModifyGestureRecognizer().m_RecognizeTap = true;
	//ModifyGestureRecognizer().m_RecognizeDoubleTap = true;

	// HACK: Recognize taps when unselected only; but this needs to be automated
	ModifyGestureRecognizer().m_RecognizeTap =    !HasTypingFocus()
											   && IsNotMinimized();
	ModifyGestureRecognizer().m_RecognizeDoubleTap =    true//HasTypingFocus()
													 && IsNotMinimized();
}

bool TextFieldWidget::IsNotMinimized() const
{
	return (   nullptr == m_MinimizeToggle
			|| m_MinimizeToggle->GetState());
}

void TextFieldWidget::ProcessTimePassed(const double TimePassed)
{
	SetupGestureRecognizer();

	CompositeWidget::ProcessTimePassed(TimePassed);		// For DraggablePositionBehavior::ProcessTimePassed() to get called
}

const Vector2n TextFieldWidget::GetDimensions() const
{
	if (IsNotMinimized())
		return CompositeWidget::GetDimensions();
	else
	{
		return Vector2n(3 * charWidth, 1 * lineHeight);
	}
}

void TextFieldWidget::Render()
{
	// TODO: Refactor this out
	if (!m_Visible)
		return;

	//Color BackgroundColor(1.0, 1.0, 1.0);
	Color BackgroundColor = m_BackgroundColor;
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
	// HACK, TODO: Make this a single DRY const
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

	if (IsNotMinimized())
	{
		// TEST
		auto ContentWithInsertion = m_Content;
		if (!m_TypingModule.GetString().empty())
		{
			for (auto & Pointer : GetGestureRecognizer().GetConnected())
			{
				if (Pointer::VirtualCategory::POINTING == Pointer->GetVirtualCategory())
				{
					Vector2n GlobalPosition(Pointer->GetPointerState().GetAxisState(0).GetPosition(), Pointer->GetPointerState().GetAxisState(1).GetPosition());
					Vector2n LocalPosition(GlobalToLocal(GlobalPosition));
					LocalPosition = m_TypingModule.GetInsertionPosition(LocalPosition);

					auto InsertionPosition = GetNearestCaretPosition(LocalPosition);
					ContentWithInsertion.insert(InsertionPosition, m_TypingModule.GetString());
				}
			}
		}
// TEST: Disable temporarily so that m_Dependee will work
#if 0
		if (!GetWidgets().empty())
		{
			auto Autocompletion = static_cast<ContextMenuWidget<std::string> *>(GetWidgets()[0].get())->GetSelectedEntry();

			if (nullptr != Autocompletion) {
				ContentWithInsertion.insert(m_CaretPosition, *Autocompletion);
			}
		}
#endif

		// Render line highlighting
		if (nullptr != m_GetLineHighlighting)
		{
			for (uint32 LineNumber = 0; LineNumber < m_ContentLines.size(); ++LineNumber)
			{
				auto LineColor = m_GetLineHighlighting(LineNumber, m_Content.substr(m_ContentLines[LineNumber].m_StartPosition, m_ContentLines[LineNumber].m_Length));

				if (Color::WHITE != LineColor) {
					DrawBoxBorderless(GetPosition() + Vector2n(0, LineNumber * lineHeight), Vector2n(GetDimensions()[0], lineHeight), LineColor);
				}
			}
		}

		// If this is a private field (e.g., for passwords), show all characters as asterisks
		if (m_Private) {
			ContentWithInsertion.assign(m_Content.length(), '*');
		}

		OpenGLStream OpenGLStream(GetPosition(), m_Foreground);
		OpenGLStream << ContentWithInsertion.substr(0, std::min(m_CaretPosition, m_SelectionPosition));

		Vector2n CaretPosition;

		// Remember caret position (selection front case)
		if (std::min(m_CaretPosition, m_SelectionPosition) == m_CaretPosition)
		{
			CaretPosition = OpenGLStream.GetCaretPosition();
		}

		// Draw selected text as highlighted
		if (HasTypingFocus())
		{
			OpenGLStream.SetBackgroundColor(Color(static_cast<uint8>(195), 212, 242));
		}
		else
		{
			OpenGLStream.SetBackgroundColor(Color(static_cast<uint8>(240), 240, 240));
		}
		auto SelectionLength = GetSelectionLength();
		OpenGLStream << ContentWithInsertion.substr(std::min(m_CaretPosition, m_SelectionPosition), SelectionLength);
		OpenGLStream.SetBackgroundColor(Color(1.0, 1.0, 1.0));

		// Remember caret position (selection back case)
		if (std::max(m_CaretPosition, m_SelectionPosition) == m_CaretPosition)
		{
			CaretPosition = OpenGLStream.GetCaretPosition();
		}

		OpenGLStream << ContentWithInsertion.substr(std::max(m_CaretPosition, m_SelectionPosition));

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

		// TODO: Optimize by calling this function once and cache the results (instead of once per line which is pretty stupid)
		// Render line annotations
		if (nullptr != m_GetLineAnnotations)
		{
			for (uint32 LineNumber = 0; LineNumber < m_ContentLines.size(); ++LineNumber) {
				auto ExpandedLength = GetCaretPositionX(LineNumber, m_ContentLines[LineNumber].m_Length) / charWidth;
				class OpenGLStream OpenGLStream(GetPosition() + Vector2n(static_cast<uint32>(ExpandedLength + 1) * charWidth, LineNumber * lineHeight));
				OpenGLStream.SetBackgroundColor(Color(1.0, 0.9, 0.9));		// HACK: Hardcoded color of failed compilation
				OpenGLStream << m_GetLineAnnotations(LineNumber);
			}
		}

		// TODO: Optimize by calling this function once and cache the results (instead of once per line which is really stupid)
		// Render line gutters
		if (nullptr != m_GetLineGutters)
		{
			/*for (uint32 LineNumber = 0; LineNumber < m_ContentLines.size(); ++LineNumber) {
				auto LineGutter = m_GetLineGutters(LineNumber);
				class OpenGLStream OpenGLStream(GetPosition() + Vector2n(-1 - Concept::GetDimensions(LineGutter).X(), LineNumber * lineHeight));
				OpenGLStream.SetBackgroundColor(Color(1.0, 0.9, 0.9));		// HACK: Hardcoded color of failed compilation
				OpenGLStream << LineGutter;
			}*/

			auto Shell = std::unique_ptr<ShellWidget>(new ShellWidget(Vector2n::ZERO, m_TypingModule));

			// HACK: Use m_GetLineGutters to get the path
			std::string Command = "cd \'" + m_GetLineGutters(0) + "\'\ngit diff --no-ext-diff -U0 -- " + m_GetLineGutters(1) + " | grep -e \'^@@ \'";

			Shell->m_CommandWidget->SetContent(Command);
			Shell->m_ExecuteWidget->GetAction()();
			auto DiffOut = Shell->m_OutputWidget->GetContent();
			TrimLastNewline(DiffOut);

			for (uint32 LineNumber = 0; LineNumber < m_ContentLines.size(); ++LineNumber) {
				class OpenGLStream OpenGLStream(GetPosition() + Vector2n(-1 - charWidth, LineNumber * lineHeight));
				OpenGLStream.SetBackgroundColor(Color(0.9, 1, 0.9));

				// Go through each line
				// TODO: Clean up
				{
					std::stringstream ss;
					ss << DiffOut;
					std::string Line;

					for (;;)
					{
						std::getline(ss, Line);

						// Parse one go error line
						try
						{
							std::string OneChanged = "@@ -" + std::to_string(LineNumber + 1) + " +" + std::to_string(LineNumber + 1) + " @@";
							std::string OneAdded = "@@ -" + std::to_string(LineNumber + 0) + ",0 +" + std::to_string(LineNumber + 1) + " @@";

							if (Line.substr(0, OneChanged.length()) == OneChanged) {
								OpenGLStream << " ";
								DrawCircle(OpenGLStream.GetCaretPosition() + Vector2n(-charWidth, lineHeight) / 2, Vector2n(6, 4), Color::BLACK, Color::BLACK);
								break;
							} else if (Line.substr(0, OneAdded.length()) == OneAdded) {
								OpenGLStream << "+";
								break;
							}
						}
						catch (...) {}

						if (ss.eof())
							break;
					}
				}
			}
		}

		// High-level overview for zoomed out view
		if (m_GolangHighlightHighLevel)
		{
			double Alpha = 1;

			{
				auto Zero = LocalToGlobal(Vector2n::ZERO);
				auto One = LocalToGlobal(Vector2n(1000, 0));
				double Scale = (One.X() - Zero.X()) / 1000.0;

				const double Slope = -1.0 / (0.5 - 0.25);
				Alpha = Scale * Slope - (0.5 * Slope);
				if (Alpha > 1) Alpha = 1;
			}

			if (Alpha > 0)
			{
				glColor4d(0, 0.5, 0, Alpha);

				for (uint32 LineNumber = 0; LineNumber < m_ContentLines.size(); )
				{
					const std::string FuncStartSignature = "func ";
					auto FuncLineNumber = FindLineThatStartsWith(FuncStartSignature, LineNumber); if (m_ContentLines.size() == FuncLineNumber) break;
					//auto FuncEndLineNumber = FindLineThatStartsWith("}", FuncLineNumber); if (m_ContentLines.size() == FuncEndLineNumber) break;
					std::string FuncLine = m_Content.substr(m_ContentLines[FuncLineNumber].m_StartPosition, m_ContentLines[FuncLineNumber].m_Length);

					glPushMatrix();
					{
						double Scale = 8;//FuncEndLineNumber + 1 - FuncLineNumber;
						glScaled(Scale, Scale, 1);
						OglUtilsPrint(GetPosition().X() / Scale, GetPosition().Y() / Scale + (FuncLineNumber * lineHeight) / Scale, 0, LEFT, FuncLine.c_str() + FuncStartSignature.length());
					}
					glPopMatrix();

					LineNumber = FuncLineNumber + 1;
				}
			}
		}
	}
	else
	{
		OpenGLStream OpenGLStream(GetPosition(), Color(0.5, 0.5, 0.5));
		OpenGLStream << "...";
	}

	// HACK: Should create a system for this to be done automatically
	// Position toggle in the top right corner
	/*if (nullptr != m_MinimizeToggle) {
		m_MinimizeToggle->ModifyPosition().X() = 0;		// This is needed so the next CompositeWidget::GetDimensions() isn't affected by this widget
		m_MinimizeToggle->ModifyPosition().X() = GetDimensions().X() - m_MinimizeToggle->GetDimensions().X();
	}*/

	CompositeWidget::Render();
}

uint32 TextFieldWidget::FindLineThatStartsWith(const std::string Target, const uint32 StartingPoint)
{
	for (uint32 LineNumber = StartingPoint; LineNumber < m_ContentLines.size(); ++LineNumber)
	{
		if (m_ContentLines[LineNumber].m_Length >= Target.length() && m_Content.substr(m_ContentLines[LineNumber].m_StartPosition, Target.length()) == Target)
			return LineNumber;
	}

	return static_cast<uint32>(m_ContentLines.size());
}

void TextFieldWidget::ProcessTap(const InputEvent & InputEvent, Vector2n Position)
{
	g_InputManager->RequestTypingPointer(ModifyGestureRecognizer());

	// Set cursor at tapped position
	if (0 == GetSelectionLength())		// Only if there's no selection
	{
		Vector2n GlobalPosition(InputEvent.m_Pointer->GetPointerState().GetAxisState(0).GetPosition(), InputEvent.m_Pointer->GetPointerState().GetAxisState(1).GetPosition());
		Vector2n LocalPosition = GlobalToLocal(GlobalPosition);

		auto CaretPosition = GetNearestCaretPosition(LocalPosition);

		SetCaretPosition(CaretPosition, true);
	}
}

void TextFieldWidget::ProcessDoubleTap(const InputEvent & InputEvent, Vector2n Position)
{
	// HACK: This should be called earlier (not at the end of the double tap gesture, which can take a long time)
	m_SelectionPosition = m_CaretPosition;		// Reset selection (so that ProcessTap() does what I expect it to here (i.e. when I changed ProcessTap, I didn't mean to change ProcessDoubleTap's behaviour,
												// it was an uninentional mistake due to lack of automatic dependency tracking and me being unaware of ProcessTap's usage inside ProcessDoubleTap)
	ProcessTap(InputEvent, Position);

	// TODO: This isn't entirely correct behaviour, it doesn't work correctly when double-clicking on whitespace
	// DUPLICATION
	{
		// Skip non-spaces to the left
		auto LookAt = m_CaretPosition - 1;
		while (   LookAt != -1
			   && IsCoreCharacter(m_Content[LookAt]))
		{
			--LookAt;
		}

		SetCaretPosition(LookAt + 1, true);
	}
	{
		// Skip non-spaces to the right
		auto LookAt = m_CaretPosition;
		while (   LookAt < m_Content.length()
			   && IsCoreCharacter(m_Content[LookAt]))
		{
			++LookAt;
		}

		SetCaretPosition(LookAt, false);
	}
}

void TextFieldWidget::ProcessCharacter(InputEvent & InputEvent, const uint32 Character)
{
	// TEST: If minimized, don't handle editing events
	if (!IsNotMinimized())
		return;

	if (Character < 128u)
	{
		EraseSelectionIfAny();

		m_Content.insert(m_CaretPosition, 1, static_cast<uint8>(Character));
		UpdateContentLines();
		MoveCaret(+1, true);

		InputEvent.m_Handled = true;
	}
}

// TODO: Merge this with MoveCaretHorizontallyRight()
void TextFieldWidget::MoveCaretHorizontallyLeft(bool SuperActive, bool AltActive, bool ShiftActive)
{
	if (SuperActive && !AltActive)
	{
		auto LineNumber = GetLineNumber();

		SetCaretPosition(m_ContentLines[LineNumber].m_StartPosition, !ShiftActive);
	}
	else if (AltActive && !SuperActive)
	{
		{
			// Skip spaces to the left
			auto LookAt = m_CaretPosition - 1;
			while (   LookAt != -1
				   && !IsCoreCharacter(m_Content[LookAt]))
			{
				--LookAt;
			}

			// Skip non-spaces to the left
			while (   LookAt != -1
				   && IsCoreCharacter(m_Content[LookAt]))
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

void TextFieldWidget::MoveCaretHorizontallyRight(bool SuperActive, bool AltActive, bool ShiftActive)
{
	if (SuperActive && !AltActive)
	{
		auto LineNumber = GetLineNumber();

		SetCaretPosition(m_ContentLines[LineNumber].m_StartPosition + m_ContentLines[LineNumber].m_Length, !ShiftActive);
	}
	else if (AltActive && !SuperActive)
	{
		{
			// Skip spaces to the right
			auto LookAt = m_CaretPosition;
			while (   LookAt < m_Content.length()
				   && !IsCoreCharacter(m_Content[LookAt]))
			{
				++LookAt;
			}

			// Skip non-spaces to the right
			while (   LookAt < m_Content.length()
				   && IsCoreCharacter(m_Content[LookAt]))
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

void TextFieldWidget::ProcessEvent(InputEvent & InputEvent)
{
	// DECISION
	//if (CheckHover())
	// HACK
	//if (HasTypingFocus())
	/*{
		// TEST
		if (   InputEvent.HasType(InputEvent::EventType::POINTER_ACTIVATION)
			&& (   InputEvent.HasType(InputEvent::EventType::BUTTON_EVENT)
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
	}*/

	// TEST: If minimized, don't handle editing events
	if (!IsNotMinimized())
		return;

	auto SelectionLength = GetSelectionLength();

	if (InputEvent.HasType(InputEvent::EventType::BUTTON_EVENT))
	{
		auto ButtonId = InputEvent.m_InputId;
		bool Pressed = InputEvent.m_Buttons[0];		// TODO: Check if there are >1 buttons

		if (Pointer::VirtualCategory::TYPING == InputEvent.m_Pointer->GetVirtualCategory())
		{
			if (Pressed)
			{
				auto ControlActive = (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LCTRL)
									  || InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RCTRL));
				auto ShiftActive = (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LSHIFT)
									|| InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RSHIFT));
				auto SuperActive = (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LSUPER)
									|| InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RSUPER));
				auto AltActive = (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LALT)
								  || InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RALT));

				bool HandledEvent = true;		// Assume true at first

				switch (ButtonId)
				{
				case GLFW_KEY_BACKSPACE:
					{
						auto SelectionExisted = EraseSelectionIfAny();

						if (false == SelectionExisted)
						{
							MoveCaretHorizontallyLeft(SuperActive, AltActive, true);
							EraseSelectionIfAny();
						}

						UpdateContentLines();
					}
					break;
				case GLFW_KEY_DEL:
					{
						auto SelectionExisted = EraseSelectionIfAny();

						if (false == SelectionExisted)
						{
							MoveCaretHorizontallyRight(SuperActive, AltActive, true);
							EraseSelectionIfAny();
						}

						UpdateContentLines();
					}
					break;
				case GLFW_KEY_ENTER:
				case GLFW_KEY_KP_ENTER:
					{
						EraseSelectionIfAny();

						auto TabCount = GetLeadingTabCount();

						m_Content.insert(m_CaretPosition, 1, '\n');
						m_Content.insert(m_CaretPosition + 1, TabCount, '\t');
						UpdateContentLines();
						MoveCaret(+1 + TabCount, true);
					}
					break;
				case GLFW_KEY_TAB:
					{
						EraseSelectionIfAny();

						m_Content.insert(m_CaretPosition, 1, '\t');
						UpdateContentLines();
						MoveCaret(+1, true);
					}
					break;
				case GLFW_KEY_SPACE:
					{
						if (ControlActive)
						{
							// Popup autocompletion list
							if (nullptr != m_GetAutocompletions)
							{
								auto Autocompletions = m_GetAutocompletions(GetCaretPosition());
								auto AutocompletionsMenu = new ContextMenuWidget<std::string>(GetCaretLocalPosition() + Vector2n(0, lineHeight), Autocompletions);
								AutocompletionsMenu->m_DoubleTapAction = [=](Vector2n, std::vector<std::string> &){
									// TODO: Do something on double-tap, like insert the selected autocompletion into parent

									g_InputManager->RequestTypingPointer(this->ModifyGestureRecognizer());
									InputEvent.m_Pointer->ModifyPointerMapping().RemoveMapping(this->ModifyGestureRecognizer());
									this->RemoveWidget(AutocompletionsMenu);		// TODO: Maybe this is wrong to do, verify
								};
								AddWidget(AutocompletionsMenu);
								g_InputManager->RequestTypingPointer(AutocompletionsMenu->ModifyGestureRecognizer());
							}
							else
							{
								HandledEvent = false;
							}
						}
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
							MoveCaretHorizontallyLeft(SuperActive, AltActive, ShiftActive);
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
							MoveCaretHorizontallyRight(SuperActive, AltActive, ShiftActive);
						}
					}
					break;
				case GLFW_KEY_UP:
					{
						if (0 != SelectionLength && !ShiftActive)
						{
							SetCaretPosition(std::min(m_CaretPosition, m_SelectionPosition), true);
						}

						if (SuperActive)
						{
							SetCaretPosition(0, !ShiftActive);		// Go to home
						}
						else
						{
							MoveCaretVerticallyTry(-1, !ShiftActive);
						}
					}
					break;
				case GLFW_KEY_DOWN:
					{
						if (0 != SelectionLength && !ShiftActive)
						{
							SetCaretPosition(std::max(m_CaretPosition, m_SelectionPosition), true);
						}

						if (SuperActive)
						{
							SetCaretPosition(m_Content.length(), !ShiftActive);		// Go to end
						}
						else
						{
							MoveCaretVerticallyTry(+1, !ShiftActive);
						}
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
						if (!m_Private)
						{
							if (SuperActive)
							{
								if (!GetSelectionContent().empty())
								{
#if DECISION_USE_CLIPBOARD_INSTEAD_OF_TypingModule
									glfwSetClipboardString(GetSelectionContent());
#else
									m_TypingModule.SetString(GetSelectionContent());
#endif

									EraseSelectionIfAny();
									UpdateContentLines();
								}
							}
						}
					}
					break;
				case 'C':
					{
						if (!m_Private)
						{
							if (SuperActive)
							{
								if (!GetSelectionContent().empty())
								{
#if DECISION_USE_CLIPBOARD_INSTEAD_OF_TypingModule
									glfwSetClipboardString(GetSelectionContent());
#else
									m_TypingModule.SetString(GetSelectionContent());
#endif
								}
							}
						}
					}
					break;
				case 'V':
					{
						if (SuperActive)
						{
							if (!glfwGetClipboardString().empty())
							{
								EraseSelectionIfAny();

#if DECISION_USE_CLIPBOARD_INSTEAD_OF_TypingModule
								m_Content.insert(m_CaretPosition, glfwGetClipboardString());
								UpdateContentLines();
								MoveCaret(static_cast<sint32>(glfwGetClipboardString().length()), true);
#else
								auto Entry = m_TypingModule.TakeString();

								m_Content.insert(m_CaretPosition, Entry);
								UpdateContentLines();
								MoveCaret(static_cast<sint32>(Entry.length()), true);
#endif
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
		else if (Pointer::VirtualCategory::POINTING == InputEvent.m_Pointer->GetVirtualCategory())
		{
			if (HasTypingFocus())
			{
				if (Pressed)
				{
					// If the context menu is visible and mouse goes down outside it, close it
					// TODO: Make this general, it should also close when widget loses focus, etc.
// TEST: Disable temporarily so that m_Dependee will work
#if 0
					{
						if (   !GetWidgets().empty()
							&& GetWidgets()[0]->GetGestureRecognizer().GetConnected().end() == GetWidgets()[0]->GetGestureRecognizer().GetConnected().find(InputEvent.m_Pointer))
						{
							g_InputManager->RequestTypingPointer(ModifyGestureRecognizer());
							InputEvent.m_Pointer->ModifyPointerMapping().RemoveMapping(GetWidgets()[0]->ModifyGestureRecognizer());
							RemoveWidget(GetWidgets()[0].get());
						}
					}
#endif

					bool HandledEvent = true;		// Assume true at first

					switch (ButtonId)
					{
					case 0:
						{
							Vector2n GlobalPosition(InputEvent.m_Pointer->GetPointerState().GetAxisState(0).GetPosition(), InputEvent.m_Pointer->GetPointerState().GetAxisState(1).GetPosition());
							Vector2n LocalPosition = GlobalToLocal(GlobalPosition);
							LocalPosition = m_TypingModule.GetInsertionPosition(LocalPosition);

							auto CaretPosition = GetNearestCaretPosition(LocalPosition);

							auto ShiftActive = g_InputManager->m_TypingPointer->GetPointerState().GetButtonState(GLFW_KEY_LSHIFT) || g_InputManager->m_TypingPointer->GetPointerState().GetButtonState(GLFW_KEY_RSHIFT);
							SetCaretPosition(CaretPosition, !ShiftActive);

							{
								auto Entry = m_TypingModule.TakeString();

								if (!Entry.empty())
								{
									m_Content.insert(m_CaretPosition, Entry);
									UpdateContentLines();
									SetCaretPosition(GetNearestCaretPosition(LocalPosition), true);
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

	if (   InputEvent.HasType(InputEvent::EventType::AXIS_EVENT)
		|| InputEvent.HasType(InputEvent::EventType::CANVAS_MOVED_TEST))
	{
		if (Pointer::VirtualCategory::POINTING == InputEvent.m_Pointer->GetVirtualCategory())
		{
			if (HasTypingFocus())
			{
				if (true == InputEvent.m_Pointer->GetPointerState().GetButtonState(0))
				{
					Vector2n GlobalPosition(InputEvent.m_Pointer->GetPointerState().GetAxisState(0).GetPosition(), InputEvent.m_Pointer->GetPointerState().GetAxisState(1).GetPosition());
					Vector2n LocalPosition = GlobalToLocal(GlobalPosition);

					auto CaretPosition = GetNearestCaretPosition(LocalPosition);

					SetCaretPosition(CaretPosition, false);

					InputEvent.m_Handled = true;
				}
			}
		}
	}
}

std::string TextFieldWidget::GetContent() const
{
	return m_Content;
}

void TextFieldWidget::SetContent(std::string Content)
{
	if (m_CaretPosition > Content.length())
		SetCaretPosition(Content.length(), false);
	if (m_SelectionPosition > Content.length())
		m_SelectionPosition = Content.length();
	//SetCaretPosition(0, true);		// Reset caret position to home

	m_Content = Content;
	UpdateContentLines();
}

void TextFieldWidget::AppendContent(std::string ExtraContent)
{
	m_Content += ExtraContent;
	UpdateContentLines();
}

decltype(TextFieldWidget::m_CaretPosition) TextFieldWidget::GetCaretPosition() const
{
	return m_CaretPosition;
}

void TextFieldWidget::SetBackground(Color BackgroundColor)
{
	m_BackgroundColor = BackgroundColor;
}

Color TextFieldWidget::GetBackground() const
{
	return m_BackgroundColor;
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
		std::vector<ContentLine>::size_type LineNumber, ColumnNumber;
		GetLineAndColumnNumber(LineNumber, ColumnNumber);

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
	std::vector<ContentLine>::size_type LineNumber, ColumnNumber;
	GetLineAndColumnNumber(LineNumber, ColumnNumber);

	if (-1 == MoveAmount)
	{
		if (LineNumber > 0)
			--LineNumber;
		else
			m_TargetCaretColumnX = 0;		// Go to beginning of first line
	}
	else if (+1 == MoveAmount)
	{
		if (LineNumber < m_ContentLines.size() - 1)
			++LineNumber;
		else
			m_TargetCaretColumnX = GetCaretPositionX(LineNumber, m_ContentLines[LineNumber].m_Length);		// Go to end of last line
	}
	//m_CaretPosition = std::min(m_ContentLines[LineNumber].m_StartPosition + ColumnNumber, m_ContentLines[LineNumber].m_StartPosition + m_ContentLines[LineNumber].m_Length);
	{
		auto CaretPosition = GetNearestCaretPosition(LineNumber, m_TargetCaretColumnX);

		SetCaretPosition(CaretPosition, ResetSelection, false);
	}
}

decltype(TextFieldWidget::m_CaretPosition) TextFieldWidget::GetSelectionLength() const
{
	return std::max(m_CaretPosition, m_SelectionPosition) - std::min(m_CaretPosition, m_SelectionPosition);
}

std::string TextFieldWidget::GetSelectionContent() const
{
	auto SelectionLength = GetSelectionLength();

	return m_Content.substr(std::min(m_CaretPosition, m_SelectionPosition), SelectionLength);
}

// Returns true if there was a selection, false otherwise
bool TextFieldWidget::EraseSelectionIfAny()
{
	auto SelectionLength = GetSelectionLength();

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

	NotifyChange();
}

void TextFieldWidget::NotifyChange(bool OverrideLiveToggle)
{
	if (nullptr != m_OnChange) {
		m_OnChange();
	}
	for (auto ConnectionWidget : GetConnected()) {
		ConnectionWidget->NotifyChange(OverrideLiveToggle);
	}

	// HACK: Manually call NotifyChange() of parent if it happens to be a TextFileWidget... this doesn't quite scale, need to figure out a better way
	auto Parent = dynamic_cast<TextFileWidget *>(ModifyParent());
	if (nullptr != Parent) {
		Parent->NotifyChange(OverrideLiveToggle);
	}
}

// Gets expanded column number in pixels (i.e. divide by charWidth to get in characters)
uint32 TextFieldWidget::GetCaretPositionX(std::vector<ContentLine>::size_type LineNumber, std::vector<ContentLine>::size_type ColumnNumber) const
{
	uint32 CaretPositionX = 0;

	{
		std::string PartialLine = m_Content.substr(m_ContentLines[LineNumber].m_StartPosition, ColumnNumber);

		std::string::size_type Start = 0, End;
		do
		{
			End = PartialLine.find_first_of('\t', Start);

			auto Length = ((std::string::npos != End) ? End : PartialLine.length()) - Start;
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

decltype(TextFieldWidget::m_CaretPosition) TextFieldWidget::GetNearestCaretPosition(Vector2n LocalPosition) const
{
	if (LocalPosition.X() < 0)
		LocalPosition.X() = 0;
	if (LocalPosition.Y() < 0)
		LocalPosition.Y() = 0;

	uint32 LineNumber = static_cast<uint32>(LocalPosition.Y()) / lineHeight;

	return GetNearestCaretPosition(LineNumber, static_cast<uint32>(LocalPosition.X()));
}

decltype(TextFieldWidget::m_CaretPosition) TextFieldWidget::GetNearestCaretPosition(std::vector<ContentLine>::size_type LineNumber, uint32 LocalPositionX) const
{
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

void TextFieldWidget::GetLineAndColumnNumber(std::vector<ContentLine>::size_type & LineNumber, std::vector<ContentLine>::size_type & ColumnNumber) const
{
	LineNumber = 0;
	ColumnNumber = 0;

	for (auto & ContentLine : m_ContentLines)
	{
		if (ContentLine.m_StartPosition + ContentLine.m_Length >= m_CaretPosition)
		{
			ColumnNumber = m_CaretPosition - ContentLine.m_StartPosition;
			break;
		}

		++LineNumber;
	}
}

// Gets the line number where the caret (m_CaretPosition) is currently located
std::vector<TextFieldWidget::ContentLine>::size_type TextFieldWidget::GetLineNumber() const
{
	std::vector<ContentLine>::size_type LineNumber = 0;

	for (auto & ContentLine : m_ContentLines)
	{
		if (ContentLine.m_StartPosition + ContentLine.m_Length >= m_CaretPosition)
		{
			break;
		}

		++LineNumber;
	}

	return LineNumber;
}

const Vector2n TextFieldWidget::GetCaretLocalPosition() const
{
	std::vector<ContentLine>::size_type LineNumber, ColumnNumber;
	GetLineAndColumnNumber(LineNumber, ColumnNumber);

	return Vector2n(GetCaretPositionX(LineNumber, ColumnNumber), static_cast<sint32>(LineNumber * lineHeight));
}

uint32 TextFieldWidget::GetLeadingTabCount() const
{
	uint32 TabCount = 0;

	auto LookAt = m_ContentLines[GetLineNumber()].m_StartPosition;
	while (   LookAt < m_Content.length()
		   && '\t' == m_Content[LookAt])
	{
		++LookAt;
		++TabCount;
	}

	return std::min<uint32>(TabCount, static_cast<uint32>(m_CaretPosition - m_ContentLines[GetLineNumber()].m_StartPosition));
}

bool TextFieldWidget::IsCoreCharacter(uint8 Character)
{
	return (   ('a' <= Character && Character <= 'z')
			|| ('A' <= Character && Character <= 'Z')
			|| ('0' <= Character && Character <= '9')
			|| '_' == Character);
}
