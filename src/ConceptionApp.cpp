#include "Main.h"

TextFieldWidget * volatile g_OutputWidget = nullptr;

ConceptionApp::ConceptionApp(InputManager & InputManager)
	: App(InputManager),
	  m_CurrentProject(),
	  m_TypingModule(),
	  m_SourceWidget(nullptr),
	  m_OutputWidget(nullptr)
{
	PopulateConcepts();

	{
		auto MainCanvas = new Canvas(Vector2n(0, 0), true, true);
		//MainCanvas->MoveView(0, 336);
		MainCanvas->MoveView(1, -64);

#if 1
		{
			auto * StdIncludesList = new ListWidget<ConceptId>(Vector2n(-200, -300), m_CurrentProject.GetStdIncludes(), m_TypingModule);
			StdIncludesList->m_TapAction = [=](Vector2n LocalPosition, std::vector<ConceptId> & m_List)
				/*{
					auto Entry = m_TypingModule.TakeString();

					if (Entry.length() > 0)
					{
						auto ConceptId = FindOrCreateConcept(Entry);

						StdIncludesList->Insert(ConceptId);
					}
					else
					{
						//auto ListEntry =
						//StdIncludesList->In
					}
				};*/
				{
					auto Entry = m_TypingModule.TakeString();

					if (!Entry.empty())
					{
						auto ConceptId = FindOrCreateConcept(Entry);

						//Insert(ConceptId);

						// TEST
						auto Spot = m_List.begin() + (LocalPosition.Y() / lineHeight);
						m_List.insert(Spot, ConceptId);
					}
					else
					{
						auto ListEntry = static_cast<decltype(m_List.size())>(LocalPosition.Y() / lineHeight);

						if (ListEntry < m_List.size())
						{
							m_TypingModule.SetString(GetConcept(m_List[ListEntry]).GetContent());
							m_List.erase(m_List.begin() + ListEntry);
						}
					}
				};

			MainCanvas->AddWidget(StdIncludesList);
		}
#endif
		MainCanvas->AddWidget(new ButtonWidget(Vector2n(-100, -350), []() { std::cout << "Hi from anon func.\n"; } ));
		MainCanvas->AddWidget(new ButtonWidget(Vector2n(-60, -350), []() { std::cout << "Second button.\n"; } ));
		MainCanvas->AddWidget(new LiveFunctionWidget(Vector2n(-100, -300), m_TypingModule, m_CurrentProject));
		MainCanvas->AddWidget(new LiveFunctionWidget(Vector2n(-100, -100), m_TypingModule, m_CurrentProject));
		MainCanvas->AddWidget(m_OutputWidget = new TextFieldWidget(Vector2n(300, -200), m_TypingModule));
		MainCanvas->AddWidget(m_SourceWidget = new TextFieldWidget(Vector2n(-500, -200), m_TypingModule));

		/*auto Test = new TextFieldWidget(Vector2n(-500, -200), m_TypingModule);
		MainCanvas->AddWidget(Test);
		Test->m_OnChange = [](){ std::cout << "Change.\n"; };*/

		// DEBUG: Irregular starting state, for testing
		{
			m_CurrentProject.SetSourceOnChange(*m_SourceWidget, *m_OutputWidget);

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

		MainCanvas->AddWidget(new ConceptStringBoxWidget(Vector2n(-400, 100 + 400), m_TypingModule));

#if 1
		{
			MainCanvas->AddWidget(new ListWidget<Concept *>(Vector2n(-730 - 450, -250), Concepts, m_TypingModule));
		}
#endif

		m_Widgets.push_back(std::unique_ptr<Widget>(MainCanvas));
	}

	// Prepare and start the thread
	{
		m_CurrentProject.StartBackgroundThread();
	}

	{
		// Load program
		m_CurrentProject.LoadSampleGenProgram(*static_cast<Canvas *>(m_Widgets[0].get()));
	}
}

ConceptionApp::~ConceptionApp()
{
	CleanConcepts();

	// Clean up temporary files
#if defined(__APPLE__) && defined(__MACH__)
	system("rm ./GenProgram");
	system("./bin/gocode/gocode drop-cache");
	system("./bin/gocode/gocode close");
#endif
}

void ConceptionApp::UpdateWindowDimensions(Vector2n WindowDimensions)
{
	// TODO: This is a hack, I should create a WindowResize listener type of thing and take care within Widget itself
	static_cast<Canvas *>(m_Widgets[0].get())->SetDimensions(WindowDimensions);
}

void ConceptionApp::Render()
{
	m_CurrentProject.SomethingFromAppRenderTEST();

	App::Render();

	// TODO, LOWER_PRIORITY: Perhaps generalize TypingModule to a Renderable object (rather than Widget) and standardize back into App, removing need for overloaded Render()
	{
		m_TypingModule.Render(GetInputManager());
	}
}

void ConceptionApp::ProcessEvent(InputEvent & InputEvent)
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

bool ConceptionApp::ShouldRedrawRegardless()
{
	if (   0 == m_CurrentProject.m_BackgroundState
		&& glfwGetTime() >= m_CurrentProject.m_ProcessEndedTime + 1
		&& !GetInputManager().AnyActivePointers()
		&& GetInputManager().EmptyInputEventQueue())
		return false;		// If idle, don't redraw regardless of input
	else
		return true;		// If background thread is doing something, we should redraw
}
