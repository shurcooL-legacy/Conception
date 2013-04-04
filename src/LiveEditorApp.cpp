#include "Main.h"

LiveEditorApp::LiveEditorApp(InputManager & InputManager)
	: App(InputManager),
	  m_CurrentProject(),
	  m_TypingModule()
{
	assert(nullptr == m_SourceWidget);
	assert(nullptr == m_OutputWidget);
	assert(nullptr == m_LiveToggle);

	PopulateConcepts();

	{
		// Add Toolbar
		{
			auto Toolbar = new CanvasWidget(Vector2n(0, 0), false, false);

			// Auto compile toggle
			{
				Toolbar->AddWidget(new FlowLayoutWidget(Vector2n(1, 1), {
					std::shared_ptr<Widget>(m_LiveToggle = new ToggleWidget(Vector2n::ZERO, [&](bool State) { if (State) { m_SourceWidget->m_OnChange(); } }, true)),
					std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("Live Compilation & Execution")))
				}, {}));
				//m_LiveToggle->UpdateHACK();
			}

			m_Widgets.push_back(std::unique_ptr<Widget>(Toolbar));
		}

		auto LeftCanvas = new CanvasWidget(Vector2n(0, 16+2), false, true, CanvasWidget::BehaviourScrolling::VerticalOnly);
		auto RightCanvas = new CanvasWidget(Vector2n(0, 16+2), false, true, CanvasWidget::BehaviourScrolling::VerticalOnly);

#if DECISION_USE_CPP_INSTEAD_OF_GO
#		error Not implemented.
#else
		LeftCanvas->AddWidget(m_SourceWidget = new TextFileWidget(Vector2n(1, 1), "./GoLand/src/TestProgram.go", m_TypingModule));
#endif
		RightCanvas->AddWidget(m_OutputWidget = new ProgramWidget(Vector2n(1, 1), m_TypingModule, m_CurrentProject, m_SourceWidget->m_TextFieldWidget));

		{
			m_SourceWidget->m_OnChange = [=]()
			{
				// LiveEditorApp resizing stuff
				if (nullptr != LeftCanvas && nullptr != RightCanvas)
				{
					LeftCanvas->ModifyDimensions().X() = m_SourceWidget->GetPosition().X() + m_SourceWidget->GetDimensions().X() + 1;
					RightCanvas->ModifyPosition().X() = m_SourceWidget->GetPosition().X() + m_SourceWidget->GetDimensions().X() + 1;
				}

				// Live Toggle effect
				if (nullptr != m_LiveToggle && !m_LiveToggle->GetState())
				{
					m_OutputWidget->SetTarget(nullptr);
					m_OutputWidget->m_Visible = false;
				}
				else
				{
					m_OutputWidget->SetTarget(m_SourceWidget->m_TextFieldWidget);
					m_OutputWidget->m_Visible = true;
				}
			};
			m_SourceWidget->m_OnChange();

			m_SourceWidget->m_TextFieldWidget->m_GetAutocompletions = GetAutocompletions("./GoLand/src/TestProgram.go");
		}

		m_Widgets.push_back(std::unique_ptr<Widget>(LeftCanvas));
		m_Widgets.push_back(std::unique_ptr<Widget>(RightCanvas));

		// Cmd+R Run shortcut
		// TODO: Doesn't work now because we have no control over `go run` here, it happens in ProgramWidget
		//m_SourceWidget->ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('R', PointerState::Modifiers::Super, m_CurrentProject.GetSourceOnChange(LeftCanvas, RightCanvas, nullptr), "Run/Refresh"));
		m_SourceWidget->ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('R', PointerState::Modifiers::Super, [=]() { m_SourceWidget->m_TextFieldWidget->NotifyChange(); }, "Run/Refresh"));

		g_InputManager->RequestTypingPointer(m_SourceWidget->m_TextFieldWidget->ModifyGestureRecognizer());		// Activate source widget for editing on startup
	}

	// Prepare and start the thread
	{
		m_CurrentProject.StartBackgroundThread();
	}
}

LiveEditorApp::~LiveEditorApp()
{
	CleanConcepts();

	// Clean up temporary files
#if (defined(__APPLE__) && defined(__MACH__)) || defined(__linux)
	system("rm ./GenProgram");
	system("rm ./GenProgram.go");
	system("./bin/gocode/gocode drop-cache");
	system("./bin/gocode/gocode close");
#endif
}

void LiveEditorApp::UpdateWindowDimensions(Vector2n WindowDimensions)
{
	// TODO: This is a hack, I should create a WindowResize listener type of thing and take care within Widget itself
	static_cast<CanvasWidget *>(m_Widgets[0].get())->SetDimensions(Vector2n(WindowDimensions.X(), 16+2));		// Toolbar

	static_cast<CanvasWidget *>(m_Widgets[1].get())->SetDimensions(Vector2n(m_SourceWidget->GetPosition().X() + m_SourceWidget->GetDimensions().X() + 1, WindowDimensions.Y()));
	static_cast<CanvasWidget *>(m_Widgets[2].get())->SetDimensions(WindowDimensions);

	//m_OutputWidget->SetPosition(Vector2n(WindowDimensions.X() / 2, 0));
}

void LiveEditorApp::Render()
{
	App::Render();

	// TODO, LOWER_PRIORITY: Perhaps generalize TypingModule to a Renderable object (rather than Widget) and standardize back into App, removing need for overloaded Render()
	{
		m_TypingModule.Render(GetInputManager());
	}
}

// DEBUG: This never gets called... so get rid of it, or make it work?
void LiveEditorApp::ProcessEvent(InputEvent & InputEvent)
{
	// DEBUG, TEST: System key handling
	if (false == InputEvent.m_Handled)
	{
		if (InputEvent.HasType(InputEvent::EventType::BUTTON_EVENT))
		{
			if (Pointer::VirtualCategory::TYPING == InputEvent.m_Pointer->GetVirtualCategory())
			{
				auto ButtonId = InputEvent.m_InputId;
				bool Pressed = InputEvent.m_Buttons[0];		// TODO: Check if there are >1 buttons

				if (Pressed)
				{
					switch (ButtonId)
					{
					//case GLFW_KEY_F5:
					case 'R':
						if (   InputEvent.m_PostEventState.GetButtonState(GLFW_KEY_LSUPER)
							|| InputEvent.m_PostEventState.GetButtonState(GLFW_KEY_RSUPER))
						{
							/*m_CurrentProject.GenerateProgram(m_SourceWidget->GetContent());
							m_OutputWidget->SetContent(m_CurrentProject.RunProgram(m_OutputWidget));*/
							//m_SourceWidget->m_OnChange();
							// TODO
							printf("Cmd+R in LiveEditorApp: This needs attention... it doesn't do what it used to anymore cuz now we need to call ProgramWidget's NotifyChange()\n");

							InputEvent.m_Handled = true;
						}
						break;
					// TEST
					/*case 'B':
						//if (glfwGetKey(GLFW_KEY_LCTRL) || glfwGetKey(GLFW_KEY_RCTRL))
						{
							m_CurrentProject.GetStdIncludes().push_back(FindOrCreateConcept("test"));

							InputEvent.m_Handled = true;
						}
						break;*/
					default:
						break;
					}
				}
			}
		}
	}

	App::ProcessEvent(InputEvent);

	// TODO, LOWER_PRIORITY: Perhaps generalize and standardize this back into App, removing need for overloaded ProcessEvent()
	if (false == InputEvent.m_Handled)
	{
		if (InputEvent.HasType(InputEvent::EventType::CHARACTER_EVENT))
		{
			if (Pointer::VirtualCategory::TYPING == InputEvent.m_Pointer->GetVirtualCategory())
			{
				auto Character = InputEvent.m_InputId;

				m_TypingModule.ProcessCharacter(InputEvent, Character);
			}
		}

		m_TypingModule.ProcessEvent(InputEvent);
	}
}

bool LiveEditorApp::ShouldRedrawRegardless()
{
	if (   0 == m_CurrentProject.m_BackgroundState
		&& glfwGetTime() >= m_CurrentProject.m_ProcessEndedTime + 1
		&& !GetInputManager().AnyActivePointers()
		&& GetInputManager().EmptyInputEventQueue())
		return false;		// If idle, don't redraw regardless of input
	else
		return true;		// If background thread is doing something, we should redraw
}

std::string LiveEditorApp::GetTitle()
{
	return "Conception: Live Editor App";
}
