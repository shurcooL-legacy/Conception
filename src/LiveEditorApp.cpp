#include "Main.h"

LiveEditorApp::LiveEditorApp(InputManager & InputManager)
	: App(InputManager),
	  m_CurrentProject(),
	  m_TypingModule(),
	  m_SourceWidget(nullptr),
	  m_OutputWidget(nullptr),
	  m_LiveToggle(nullptr)
{
	PopulateConcepts();

	{
		// Add Toolbar
		{
			auto Toolbar = new Canvas(Vector2n(0, 0), false, false);

			// Auto compile toggle
			{
				Toolbar->AddWidget(new FlowLayoutWidget(Vector2n(1, 1), { std::shared_ptr<Widget>(m_LiveToggle = new ToggleWidget(Vector2n::ZERO, [](bool State){}, true)), std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("Live Compilation & Execution"))) }, {} ));
				//static_cast<ToggleWidget *>(Toolbar->GetWidgets()[0].get())->UpdateHACK();
			}

			m_Widgets.push_back(std::unique_ptr<Widget>(Toolbar));
		}

		auto LeftCanvas = new Canvas(Vector2n(0, 16+2), false, true, Canvas::BehaviourScrolling::VerticalOnly);
		auto RightCanvas = new Canvas(Vector2n(0, 16+2), false, true, Canvas::BehaviourScrolling::VerticalOnly);

		LeftCanvas->AddWidget(m_SourceWidget = new TextFieldWidget(Vector2n(1, 1), m_TypingModule));
		RightCanvas->AddWidget(m_OutputWidget = new TextFieldWidget(Vector2n(1, 1), m_TypingModule));

		{
			m_CurrentProject.SetSourceOnChange(*m_SourceWidget, *m_OutputWidget, LeftCanvas, RightCanvas, m_LiveToggle);

			m_SourceWidget->m_GetAutocompletions = [&]() -> std::vector<std::string>
			{
				std::vector<std::string> Autocompletions;

				//Autocompletions = { "Line 1", "Line 2", "Line 3" };

				// Get autocompletion using gocode
				std::string Output = "";
				{
					int PipeFd[2];
					pipe(PipeFd);
					fcntl(PipeFd[0], F_SETFL, O_NONBLOCK);
					std::cout << "gocode: Opened " << PipeFd[0] << " and " << PipeFd[1] << ".\n";

					uint8 ProcessResult;

					{
						auto Pid = fork();

						if (0 == Pid)
						{
							close(PipeFd[0]);    // close reading end in the child

							dup2(PipeFd[1], 1);  // send stdout to the pipe
							dup2(PipeFd[1], 2);  // send stderr to the pipe

							close(PipeFd[1]);    // this descriptor is no longer needed

							execl("./bin/gocode/gocode", "./bin/gocode/gocode", "-f=nice", "-in=./GenProgram.go", "autocomplete", "./GenProgram.go", std::to_string(m_SourceWidget->GetCaretPosition()).c_str(), (char *)0);

							//exit(1);		// Not needed, just in case I comment out the above
						}
						else if (-1 == Pid)
						{
							std::cerr << "Error forking.\n";
							throw 0;
						}
						else
						{
							// Wait for child process to complete
							{
								int status;
								waitpid(Pid, &status, 0);
								Pid = 0;

								std::cout << "Child finished with status " << status << ".\n";

								ProcessResult = static_cast<uint8>(status >> 8);
							}

							// Read output from pipe and put it into Output
							if (0 == ProcessResult)
							{
								char buffer[1024];
								ssize_t n;
								while (0 != (n = read(PipeFd[0], buffer, sizeof(buffer))))
								{
									if (-1 == n) {
										if (EAGAIN == errno) {
											break;
										} else {
											std::cerr << "Error: Reading from pipe " << PipeFd[0] << " failed with errno " << errno << ".\n";
											break;
										}
									}
									else
									{
										Output.append(buffer, n);
									}
								}
							}

							std::cout << "Done in parent!\n";
						}
					}

					close(PipeFd[0]);
					close(PipeFd[1]);
				}

				// Parse Output and populate Autocompletions
				// TODO: Clean up
				{
					std::stringstream ss;
					ss << Output;
					std::string Line;

					std::getline(ss, Line);		// Skip first line
					std::getline(ss, Line);
					while (!Line.empty() && !ss.eof())
					{
						Autocompletions.push_back(Line);
						std::getline(ss, Line);
					}
					if (!Line.empty())
						Autocompletions.push_back(Line);
				}

				return Autocompletions;
			};

#if DECISION_USE_CPP_INSTEAD_OF_GO
			m_SourceWidget->SetContent(FromFileToString("./GenProgram.cpp"));
#else
			m_SourceWidget->SetContent(FromFileToString("./GenProgram.go"));
#endif
		}

		m_Widgets.push_back(std::unique_ptr<Widget>(LeftCanvas));
		m_Widgets.push_back(std::unique_ptr<Widget>(RightCanvas));

		g_InputManager->RequestTypingPointer(m_SourceWidget->ModifyGestureRecognizer());		// Activate source widget for editing on startup
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
	system("./bin/gocode/gocode drop-cache");
	system("./bin/gocode/gocode close");
#endif
}

void LiveEditorApp::UpdateWindowDimensions(Vector2n WindowDimensions)
{
	// TODO: This is a hack, I should create a WindowResize listener type of thing and take care within Widget itself
	static_cast<Canvas *>(m_Widgets[0].get())->SetDimensions(Vector2n(WindowDimensions.X(), 16+2));		// Toolbar

	static_cast<Canvas *>(m_Widgets[1].get())->SetDimensions(Vector2n(m_SourceWidget->GetPosition().X() + m_SourceWidget->GetDimensions().X() + 1, WindowDimensions.Y()));
	static_cast<Canvas *>(m_Widgets[2].get())->SetDimensions(WindowDimensions);

	//m_OutputWidget->SetPosition(Vector2n(WindowDimensions.X() / 2, 0));
}

void LiveEditorApp::Render()
{
	m_CurrentProject.SomethingFromAppRenderTEST();

	App::Render();

	// TODO, LOWER_PRIORITY: Perhaps generalize TypingModule to a Renderable object (rather than Widget) and standardize back into App, removing need for overloaded Render()
	{
		m_TypingModule.Render(GetInputManager());
	}
}

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
						if (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LSUPER)
							|| InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RSUPER))
						{
							/*m_CurrentProject.GenerateProgram(m_SourceWidget->GetContent());
							m_OutputWidget->SetContent(m_CurrentProject.RunProgram(m_OutputWidget));*/
							m_SourceWidget->m_OnChange();

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
