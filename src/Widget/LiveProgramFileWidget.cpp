#include "../Main.h"

LiveProgramFileWidget::LiveProgramFileWidget(Vector2n Position, std::string Path, TypingModule & TypingModule, Project & Project)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(m_SourceFileWidget = new TextFileWidget(Vector2n::ZERO, Path, TypingModule)),
								   std::shared_ptr<Widget>(m_ProgramWidget = new ProgramWidget(Vector2n::ZERO, TypingModule, Project, m_SourceFileWidget->m_TextFieldWidget)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) }),
	  m_SourceWidget(m_SourceFileWidget->m_TextFieldWidget)
{
	m_SourceFileWidget->RemoveAllBehaviors();
	m_ProgramWidget->RemoveAllBehaviors();

	m_SourceWidget->m_GetAutocompletions = [&]() -> std::vector<std::string>
	{
		std::vector<std::string> Autocompletions;

#if 0
		Autocompletions = { "Line 1", "Line 2", "Line 3" };
#else
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

					// TODO: Should I be using the ./GenProgram.go file or something else?
					execl("./bin/gocode/gocode", "./bin/gocode/gocode", "-f=nice", "-in=./GenProgram.go", "autocomplete", "./GenProgram.go", std::to_string(m_SourceWidget->GetCaretPosition()).c_str(), (char *)0);

					//exit(1);		// Not needed, just in case I comment out the above
					throw 0;
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
#endif

		return Autocompletions;
	};

	m_SourceWidget->m_GetLineAnnotations = [=](uint32 LineNumber) -> std::string
	{
		//return std::to_string(LineNumber + 1) + "-" + (m_ProgramWidget->m_OutputWidget->GetContent().length() >= 3 ? m_ProgramWidget->m_OutputWidget->GetContent().substr(0, 3) : "...");

		// HACK: Using hardcoded color, make this better
		// Don't do this if the result is not compile error
		if (Color(1.0, 0.9, 0.9) != m_ProgramWidget->m_OutputWidget->GetBackground())
			return "";

		// TODO: Clean up
		{
			std::stringstream ss;
			auto Input = m_ProgramWidget->m_OutputWidget->GetContent();
			TrimLastNewline(Input);
			ss << Input;
			std::string Line;

			std::getline(ss, Line);		// Skip first line
			for (;;)
			{
				std::getline(ss, Line);

				// Parse one go error line
				try
				{
					auto FirstColon = Line.find(':');
					auto SecondColon = Line.find(':', FirstColon + 1);
					uint32 FoundLineNumber = std::stoi(Line.substr(FirstColon + 1, SecondColon - (FirstColon + 1)));

					if (FoundLineNumber == LineNumber + 1)
						return TrimFirstSpace(Line.substr(SecondColon + 1));
				}
				catch (...) {}

				if (ss.eof())
					break;
			}
		}
		return "";
	};

#if 0
#if DECISION_USE_CPP_INSTEAD_OF_GO
	m_SourceWidget->SetContent(FromFileToString("./GenProgram.cpp"));
#else
	m_SourceWidget->SetContent(FromFileToString("./GenProgram.go"));
#endif
#endif

	ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('R', PointerState::Modifiers::Super, [=]() { m_SourceWidget->NotifyChange(); } ));
}

LiveProgramFileWidget::~LiveProgramFileWidget()
{
}
