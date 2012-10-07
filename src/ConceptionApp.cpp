#include "Main.h"

#include <fcntl.h>

ConceptionApp::ConceptionApp(InputManager & InputManager)
	: App(InputManager),
	  m_CurrentProject(),
	  m_TypingModule(),
	  m_SourceWidget(nullptr),
	  m_OutputWidget(nullptr),
	  m_BackgroundState(0),
	  m_LastPid(0),
	  m_ExpiredOutput(false),
	  m_PipeFd(),
	  m_BackgroundThread(&ConceptionApp::BackgroundThread, this, "Background")
{
	m_PipeFd[0] = m_PipeFd[1] = -1;

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
		MainCanvas->AddWidget(m_OutputWidget = new TextFieldWidget(Vector2n(200, -200), m_TypingModule));
		MainCanvas->AddWidget(m_SourceWidget = new TextFieldWidget(Vector2n(-400, -200), m_TypingModule));

		/*auto Test = new TextFieldWidget(Vector2n(-500, -200), m_TypingModule);
		MainCanvas->AddWidget(Test);
		Test->m_OnChange = [](){ std::cout << "Change.\n"; };*/

		// DEBUG: Irregular starting state, for testing
		{
			m_SourceWidget->m_OnChange = [&]()
			{
				//printf("m_SourceWidget->m_OnChange\n");
				//m_OutputWidget->SetContent(m_OutputWidget->GetContent() + "+");

				m_CurrentProject.GenerateProgram(m_SourceWidget->GetContent());
				/*uint8 Status;
				m_OutputWidget->SetContent(m_CurrentProject.RunProgram(Status));
				if (0 == Status)
					m_OutputWidget->SetBackground(Color(1.0, 1, 1));
				else
					m_OutputWidget->SetBackground(Color(1.0, 0, 0));*/

				//m_CurrentProject.RunProgram(m_OutputWidget);

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
				m_ExpiredOutput = true;
				m_BackgroundState = 1;
			};

			//m_Content = "int main(int argc, char * argv[])\n{\n\tPrintHi();\n\treturn 0;\n}";
			m_SourceWidget->SetContent(
#if 0
				"{""\n"
				"	// Skip non-spaces to the right""\n"
				"	auto LookAt = m_CaretPosition;""\n"
				"	while (   LookAt < m_Content.length()""\n"
				"		   && IsCoreCharacter(m_Content[LookAt]))""\n"
				"	{""\n"
				"		++LookAt;""\n"
				"	}""\n"
				"""\n"
				"	SetCaretPosition(LookAt, false);""\n"
				"}"
#else
				FromFileToString("GenProgram.go")
#endif
			);
		}

		MainCanvas->AddWidget(new ConceptStringBoxWidget(Vector2n(-400, 100 + 400), m_TypingModule));

#if 1
		{
			MainCanvas->AddWidget(new ListWidget<Concept *>(Vector2n(-730 - 300, -250), Concepts, m_TypingModule));
		}
#endif

		m_Widgets.push_back(std::unique_ptr<Widget>(MainCanvas));
	}

	// Prepare and start the thread
	{
		m_BackgroundThread.Start();
	}

	{
		// Load program
		m_CurrentProject.LoadSampleGenProgram(*static_cast<Canvas *>(m_Widgets[0].get()));
	}
}

ConceptionApp::~ConceptionApp()
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

void GLFWCALL ConceptionApp::BackgroundThread(void * pArgument)
{
	Thread * Thread = Thread::GetThisThreadAndRevertArgument(pArgument);

	auto App = static_cast<ConceptionApp *>(pArgument);

	// Main loop
	while (Thread->ShouldBeRunning())
	{
		//std::cout << "Sleeping in background thread " << glfwGetTime() << ".\n";
		//glfwSleep(0.5);
		glfwSleep(0);

		if (0 == App->m_BackgroundState)
			continue;

		App->m_BackgroundState = 2;
		App->m_OutputWidget->SetBackground(Color(0.9, 0.9, 0.9));

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

		uint8 CompilationResult;

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
				execl("/usr/local/go/bin/go", "go", "build", "GenProgram.go", (char *)0);

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

					CompilationResult = static_cast<uint8>(status >> 8);
				}
				
				std::cout << "Done in parent!\n";
			}
		}

		if (0 == CompilationResult)
			App->m_OutputWidget->SetBackground(Color(1.0, 1, 1));
		else {
			App->m_OutputWidget->SetBackground(Color(1.0, 0, 0));
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
				execl("GenProgram", "GenProgram", (char *)0);

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
				}
				
				std::cout << "Done in parent!\n";
			}
		}

		App->m_OutputWidget->SetBackground(Color(0.0, 1, 0));

		if (2 == App->m_BackgroundState)
			App->m_BackgroundState = 0;
	}

	Thread->ThreadEnded();
}

void ConceptionApp::UpdateWindowDimensions(Vector2n WindowDimensions)
{
	// TODO: This is a hack, I should create a WindowResize listener type of thing and take care within Widget itself
	static_cast<Canvas *>(m_Widgets[0].get())->SetDimensions(WindowDimensions);
}

void ConceptionApp::Render()
{
	// TEST: This should go to ProcessTimePassed() or something
	if (-1 != m_PipeFd[0])
	{
		char buffer[1024];
		ssize_t n;
		while (0 != (n = read(m_PipeFd[0], buffer, sizeof(buffer))))
		{
			if (-1 == n) {
				if (EAGAIN == errno)
					break;
				else {
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
	}

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
					/*case 'R':
						if (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LSUPER)
							|| InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RSUPER))
						{
							m_CurrentProject.GenerateProgram(m_SourceWidget->GetContent());
							m_OutputWidget->SetContent(m_CurrentProject.RunProgram(m_OutputWidget));

							InputEvent.m_Handled = true;
						}
						break;*/
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
