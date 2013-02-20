#include "../Main.h"

ShellWidget::ShellWidget(Vector2n Position, TypingModule & TypingModule)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(m_CommandWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)),
								   std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("bash"), LabelWidget::Background::Normal)),
								   //std::shared_ptr<Widget>(m_ExecuteWidget = new ButtonWidget(Vector2n::ZERO, Vector2n(16, 16), [&](){} )),
								   std::shared_ptr<Widget>(m_OutputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
	{
		m_ExecuteWidget = new ButtonWidget(Vector2n::ZERO, Vector2n(16, 16), [&](){} );

		m_ExecuteWidget->SetAction([&]()
		{
			std::string Output = "";
			{
				int PipeFd[2];			// Pipe for reading from child's stdout+stderr
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

						const char * envp[] = { "TERM=xterm", nullptr };
						execle("/bin/bash", "/bin/bash", "-c", m_CommandWidget->GetContent().c_str(), (char *)0, envp);

						// TODO: Add error checking on above execl(), and do exit() in case execution reaches here
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

			// Find clear code, make it do its job
			{
				const char ClearCode[] = { 0x1B, 0x5B, 0x48, 0x1B, 0x5B, 0x32, 0x4A };

				auto n = Output.rfind(ClearCode, std::string::npos, sizeof(ClearCode));

				if (std::string::npos != n)
				{
					Output = Output.substr(n + sizeof(ClearCode));
				}
			}

			// Trim last newline, if there is one
			//TrimLastNewline(Output);

			m_OutputWidget->SetContent(Output);
		});
	}

	ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('R', PointerState::Modifiers::Super, m_ExecuteWidget->GetAction()));
}

ShellWidget::~ShellWidget()
{
}
