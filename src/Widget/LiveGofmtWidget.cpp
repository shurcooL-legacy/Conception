#include "../Main.h"

LiveGofmtWidget::LiveGofmtWidget(Vector2n Position, TypingModule & TypingModule, Project & Project)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(m_SourceWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)),
								   std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("gofmt"), LabelWidget::Background::Normal)),
								   std::shared_ptr<Widget>(m_OutputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) }),
	  m_Project(Project)
{
	{
		m_SourceWidget->m_OnChange = [&]()
		{
			m_Project.GenerateProgram(m_SourceWidget->GetContent());

			std::string Output = "";
			{
				int PipeFd[2];
				pipe(PipeFd);
				fcntl(PipeFd[0], F_SETFL, O_NONBLOCK);
				std::cout << "gofmt: Opened " << PipeFd[0] << " and " << PipeFd[1] << ".\n";

				uint8 ProcessResult;

				{
					auto Pid = fork();

					if (0 == Pid)
					{
						close(PipeFd[0]);    // close reading end in the child

						dup2(PipeFd[1], 1);  // send stdout to the pipe
						dup2(PipeFd[1], 2);  // send stderr to the pipe

						close(PipeFd[1]);    // this descriptor is no longer needed

						execl("/usr/local/go/bin/gofmt", "/usr/local/go/bin/gofmt", "./GenProgram.go", (char *)0);

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

			// Trim last newline, if there is one
			if (   Output.size() >= 1
				&& Output.back() == '\n')
			Output.pop_back();

			m_OutputWidget->SetContent(Output);
		};

#if DECISION_USE_CPP_INSTEAD_OF_GO
		m_SourceWidget->SetContent(FromFileToString("./GenProgram.cpp"));
#else
		m_SourceWidget->SetContent(FromFileToString("./GenProgram.go"));
#endif
	}

	m_SourceWidget->m_OnChange();
}

LiveGofmtWidget::~LiveGofmtWidget()
{
}
