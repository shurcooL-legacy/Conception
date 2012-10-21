#include "Main.h"

LiveEditorApp::LiveEditorApp(InputManager & InputManager)
	: App(InputManager),
	  m_CurrentProject(),
	  m_TypingModule(),
	  m_SourceWidget(nullptr),
	  m_OutputWidget(nullptr),
	  m_BackgroundState(0),
	  m_LastPid(0),
	  m_ProcessStartedTime(0),
	  m_ProcessEndedTime(0),
	  m_ExpiredOutput(false),
	  m_PipeFd(),
	  m_BackgroundThread(&LiveEditorApp::BackgroundThread, this, "Background")
{
	m_PipeFd[0] = m_PipeFd[1] = -1;

	PopulateConcepts();

	{
		auto LeftCanvas = new Canvas(Vector2n(0, 0), false, true, Canvas::BehaviourScrolling::VerticalOnly);
		auto RightCanvas = new Canvas(Vector2n(0, 0), false, true, Canvas::BehaviourScrolling::VerticalOnly);

		LeftCanvas->AddWidget(m_SourceWidget = new TextFieldWidget(Vector2n(1, 1), m_TypingModule));
		RightCanvas->AddWidget(m_OutputWidget = new TextFieldWidget(Vector2n(1, 1), m_TypingModule));

		{
			m_SourceWidget->m_OnChange = [&, LeftCanvas, RightCanvas]()
			{
				//printf("m_SourceWidget->m_OnChange\n");
				//m_OutputWidget->SetContent(m_OutputWidget->GetContent() + "+");

				LeftCanvas->ModifyDimensions().X() = m_SourceWidget->GetPosition().X() + m_SourceWidget->GetDimensions().X() + 1;
				RightCanvas->ModifyPosition().X() = m_SourceWidget->GetPosition().X() + m_SourceWidget->GetDimensions().X() + 1;

				m_CurrentProject.GenerateProgram(m_SourceWidget->GetContent());
				/*uint8 Status;
				m_OutputWidget->SetContent(m_CurrentProject.RunProgram(Status));
				if (0 == Status)
					m_OutputWidget->SetBackground(Color(1.0, 1, 1));
				else
					m_OutputWidget->SetBackground(Color(1.0, 0, 0));*/

				//m_CurrentProject.RunProgram(m_OutputWidget);

				m_ProcessEndedTime = glfwGetTime();
				m_BackgroundState = 0;

				// Kill child processes
				if (0 != m_LastPid)
				{
					std::cout << "Sending kill to last child pid " << m_LastPid << ".\n";
					//auto Result = kill(0, SIGTERM);
					auto Result = killpg(m_LastPid, SIGKILL);
					//waitpid(m_LastPid, NULL, 0);

					if (0 != Result) {
						std::cerr << "Error: kill() failed with return " << Result << ", errno " << errno << ".\n";
						//throw 0;
					}
				}

				std::cout << "Closing " << m_PipeFd[0] << " and " << m_PipeFd[1] << "; ";
				close(m_PipeFd[0]);		// Close the read end of the pipe in the parent
				m_PipeFd[0] = m_PipeFd[1] = -1;

				//m_OutputWidget->SetContent("");
				m_ProcessStartedTime = glfwGetTime();
				m_ExpiredOutput = true;
				m_BackgroundState = 1;
			};

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
		m_BackgroundThread.Start();
	}
}

LiveEditorApp::~LiveEditorApp()
{
	// Close pipes
	{
		close(m_PipeFd[0]);
		close(m_PipeFd[1]);

		// Kill child processes
		if (0 != m_LastPid)
		{
			std::cout << "Sending kill to last child pid " << m_LastPid << ".\n";
			//auto Result = kill(0, SIGTERM);
			auto Result = killpg(m_LastPid, SIGKILL);
			//waitpid(m_LastPid, NULL, 0);

			if (0 != Result) {
				std::cerr << "Error: kill() failed with return " << Result << ", errno " << errno << ".\n";
				//throw 0;
			}
		}
	}

	CleanConcepts();
}

void LiveEditorApp::UpdateWindowDimensions(Vector2n WindowDimensions)
{
	// TODO: This is a hack, I should create a WindowResize listener type of thing and take care within Widget itself
	static_cast<Canvas *>(m_Widgets[0].get())->SetDimensions(Vector2n(m_SourceWidget->GetPosition().X() + m_SourceWidget->GetDimensions().X() + 1, WindowDimensions.Y()));
	static_cast<Canvas *>(m_Widgets[1].get())->SetDimensions(WindowDimensions);

	//m_OutputWidget->SetPosition(Vector2n(WindowDimensions.X() / 2, 0));
}

void GLFWCALL LiveEditorApp::BackgroundThread(void * pArgument)
{
	Thread * Thread = Thread::GetThisThreadAndRevertArgument(pArgument);

	auto App = static_cast<LiveEditorApp *>(pArgument);

	// Main loop
	while (Thread->ShouldBeRunning())
	{
		//std::cout << "Sleeping in background thread " << glfwGetTime() << ".\n";
		//glfwSleep(0.5);
		glfwSleep(0.001);

		if (0 == App->m_BackgroundState)
			continue;

		App->m_BackgroundState = 2;
		App->m_OutputWidget->SetBackground(App->m_CompilingColor);

		/*auto PipeFd = App->m_PipeFd[1];
		auto Write = [&](std::string String) {
			//App->m_OutputWidget->AppendContent(String);
			write(PipeFd, String.c_str(), String.length());
		};*/

		/*Write("Compiling ");
		for (int i = 0; i < 15; ++i) {
			Write(".");
			glfwSleep(0.2);
		}
		Write(" Done.\n");*/

		close(App->m_PipeFd[1]);		// Close the write end of the pipe in the parent

		pipe(App->m_PipeFd);
		fcntl(App->m_PipeFd[0], F_SETFL, O_NONBLOCK);
		std::cout << "opened " << App->m_PipeFd[0] << " and " << App->m_PipeFd[1] << ".\n";

		uint8 ProcessResult;

		{
			App->m_LastPid = fork();

			if (0 == App->m_LastPid)
			{
				close(App->m_PipeFd[0]);    // close reading end in the child

				dup2(App->m_PipeFd[1], 1);  // send stdout to the pipe
				dup2(App->m_PipeFd[1], 2);  // send stderr to the pipe

				close(App->m_PipeFd[1]);    // this descriptor is no longer needed

				//execl("/bin/echo", "echo", "-n", "hello", "there,", "how are you?", (char *)0);
				//execl("/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/print-args", "echo", "-n", "hello", "there,", "how are you?", (char *)0);
				//execl("/usr/local/go/bin/go", "go", "version", (char *)0);
#if DECISION_USE_CPP_INSTEAD_OF_GO
				execl("/usr/bin/clang++", "/usr/bin/clang++", "./GenProgram.cpp", "-o", "./GenProgram", (char *)0);
#else
				execl("/usr/local/go/bin/go", "/usr/local/go/bin/go", "build", "./GenProgram.go", (char *)0);
#endif

				//exit(1);		// Not needed, just in case I comment out the above
			}
			else if (-1 == App->m_LastPid)
			{
				std::cerr << "Error forking.\n";
				throw 0;
			}
			else
			{
				std::cout << "Before: " << getpgid(App->m_LastPid) << ".\n";
				//setpgrp();
				setpgid(App->m_LastPid, App->m_LastPid);
				std::cout << "After: " << getpgid(App->m_LastPid) << ".\n";

				std::cout << "In parent, created pid " << App->m_LastPid << ".\n";

				//OutputWidget->SetBackground(Color(0.9, 0.9, 0.9));

				// Wait for child process to complete
				{
					int status;
					waitpid(App->m_LastPid, &status, 0);
					App->m_LastPid = 0;

					std::cout << "Child finished with status " << status << ".\n";

					// If killed, just skip
					if (   WIFSIGNALED(status)
						&& 9 == WTERMSIG(status))
					{
						continue;
					}

					ProcessResult = static_cast<uint8>(status >> 8);
				}
				
				std::cout << "Done in parent!\n";
			}
		}

		if (0 == ProcessResult) {
			App->m_OutputWidget->SetBackground(App->m_RunningColor);

			// HACK: This is dangerous, shouldn't modify OutputWidget contents from this thread, should send a signal to main thread, or use a mutex (but too lazy ATM to add all the code for a mutex)
			/*if (App->m_ExpiredOutput)
			{
				App->m_OutputWidget->SetContent("");
				App->m_ExpiredOutput = false;
			}*/
		} else {
			App->m_OutputWidget->SetBackground(App->m_ErrorCompileColor);
			App->m_ProcessEndedTime = glfwGetTime();
			App->m_BackgroundState = 0;
		}

		if (2 != App->m_BackgroundState)
			continue;

		{
			App->m_LastPid = fork();

			if (0 == App->m_LastPid)
			{
				close(App->m_PipeFd[0]);    // close reading end in the child

				dup2(App->m_PipeFd[1], 1);  // send stdout to the pipe
				dup2(App->m_PipeFd[1], 2);  // send stderr to the pipe

				close(App->m_PipeFd[1]);    // this descriptor is no longer needed

				//execl("/bin/echo", "echo", "-n", "hello", "there,", "how are you?", (char *)0);
				//execl("/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/print-args", "echo", "-n", "hello", "there,", "how are you?", (char *)0);
				//execl("/usr/local/go/bin/go", "go", "version", (char *)0);
				execl("./GenProgram", "./GenProgram", (char *)0);
				//execl("./gocode", "gocode", "-in=./GenProgram.go", "autocomplete", "./GenProgram.go", std::to_string(App->m_SourceWidget->GetCaretPosition()).c_str(), (char *)0);

				//exit(1);		// Not needed, just in case I comment out the above
			}
			else if (-1 == App->m_LastPid)
			{
				std::cerr << "Error forking.\n";
				throw 0;
			}
			else
			{
				std::cout << "Before: " << getpgid(App->m_LastPid) << ".\n";
				//setpgrp();
				setpgid(App->m_LastPid, App->m_LastPid);
				std::cout << "After: " << getpgid(App->m_LastPid) << ".\n";

				std::cout << "In parent, created pid " << App->m_LastPid << ".\n";

				// Wait for child process to complete
				{
					int status;
					waitpid(App->m_LastPid, &status, 0);
					App->m_LastPid = 0;

					std::cout << "Child finished with status " << status << ".\n";

					// If killed, just skip
					if (   WIFSIGNALED(status)
						&& 9 == WTERMSIG(status))
					{
						continue;
					}

					ProcessResult = static_cast<uint8>(status >> 8);
				}
				
				std::cout << "Done in parent!\n";
			}
		}

		/*uint8 Char = '\0';
		write(App->m_PipeFd[1], &Char, 1);*/

		// HACK: This is dangerous, shouldn't modify OutputWidget contents from this thread, should send a signal to main thread, or use a mutex
		if (App->m_ExpiredOutput)
		{
			App->m_OutputWidget->SetContent("");
			App->m_ExpiredOutput = false;
		}

		//close(App->m_PipeFd[0]);		// Close the read end of the pipe in the parent
		//close(App->m_PipeFd[1]);		// Close the write end of the pipe in the parent

		if (0 == ProcessResult) {
			App->m_OutputWidget->SetBackground(App->m_FinishedSuccessColor);
		} else {
			App->m_OutputWidget->SetBackground(App->m_FinishedErrorColor);
		}

		if (2 == App->m_BackgroundState) {
			App->m_ProcessEndedTime = glfwGetTime();
			App->m_BackgroundState = 0;
		}
	}

	Thread->ThreadEnded();
}

void LiveEditorApp::Render()
{
	// TEST: This should go to ProcessTimePassed() or something
	if (-1 != m_PipeFd[0])
	{
		char buffer[1024];
		ssize_t n;
		while (0 != (n = read(m_PipeFd[0], buffer, sizeof(buffer))))
		{
			if (-1 == n) {
				if (EAGAIN == errno) {
					break;
				} else {
					/*if (m_ExpiredOutput)// && 0 != m_BackgroundState)
					{
						m_OutputWidget->SetContent("EXPIRED blah");
						m_ExpiredOutput = false;
					}*/

					std::cerr << "Error: Reading from pipe " << m_PipeFd[0] << " failed with errno " << errno << ".\n";
					break;
				}
			}
			else
			{
				std::string str(buffer, n);
				if (m_ExpiredOutput)
				{
					m_OutputWidget->SetContent(str);
					m_ExpiredOutput = false;
				}
				else
				{
					m_OutputWidget->AppendContent(str);
				}
			}
		}

		// If the output is still expired after a second since process started, just clear the output
		if (glfwGetTime() >= m_ProcessStartedTime + 1.0)
		{
			if (m_ExpiredOutput)
			{
				m_OutputWidget->SetContent("");
				m_ExpiredOutput = false;
			}
		}
	}

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
		if (InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::CHARACTER_EVENT))
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
	if (0 == m_BackgroundState && glfwGetTime() >= m_ProcessEndedTime + 1)
		return false;		// If idle, don't redraw regardless of input
	else
		return true;		// If background thread is doing something, we should redraw
}

std::string LiveEditorApp::GetTitle()
{
	return "Conception: Live Editor App";
}
