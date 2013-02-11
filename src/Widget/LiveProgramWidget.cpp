#include "../Main.h"

LiveProgramWidget::LiveProgramWidget(Vector2n Position, TypingModule & TypingModule, Project & Project)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(m_SourceWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)),
								   std::shared_ptr<Widget>(m_OutputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
	{
		m_SourceWidget->m_OnChange = Project.GetSourceOnChange(*m_SourceWidget, *m_OutputWidget, nullptr, nullptr);

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
#endif

			return Autocompletions;
		};

#if DECISION_USE_CPP_INSTEAD_OF_GO
		m_SourceWidget->SetContent(FromFileToString("./GenProgram.cpp"));
#else
		m_SourceWidget->SetContent(FromFileToString("./GenProgram.go"));
#endif
	}

	ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('R', PointerState::Modifiers::Super, m_SourceWidget->m_OnChange));
}

LiveProgramWidget::~LiveProgramWidget()
{
}
