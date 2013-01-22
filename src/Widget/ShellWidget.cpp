#include "../Main.h"

ShellWidget::ShellWidget(Vector2n Position, TypingModule & TypingModule)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("bash"), LabelWidget::Background::Normal)),
								   std::shared_ptr<Widget>(m_CommandWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)),
								   std::shared_ptr<Widget>(m_ExecuteWidget = new ButtonWidget(Vector2n::ZERO, Vector2n(16, 16), [&](){} )),
								   std::shared_ptr<Widget>(m_OutputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
	{
		m_ExecuteWidget->SetAction([&]()
		{
			std::string Output = "";
			{
				int PipeFd[2];			// Pipe for reading from child's stdout+stderr
				int PipeInFd[2];		// Pipe for writing to child process stdin
				pipe(PipeFd);
				pipe(PipeInFd);
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

						close(PipeInFd[1]);    // close writing end in the child
						dup2(PipeInFd[0], 0);  // get stdin from the pipe
						close(PipeInFd[0]);    // this descriptor is no longer needed

						execl("/bin/bash", "/bin/bash", "-c", m_CommandWidget->GetContent().c_str(), (char *)0);

						// TODO: Add error checking on above execl(), and do exit() in case execution reaches here
						//exit(1);		// Not needed, just in case I comment out the above
					}
					else if (-1 == Pid)
					{
						std::cerr << "Error forking.\n";
						throw 0;
					}
					else
					{
						// Write to child's stdin and end it
						// TODO: Error check the write, perhaps need multiple tries to fully flush it
						//write(PipeInFd[1], "", 0);
						close(PipeInFd[1]);

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

						std::cout << "Done in parent!\n";
					}
				}

				close(PipeFd[0]);
				close(PipeFd[1]);
				close(PipeInFd[0]);
			}

			// Trim last newline, if there is one
			{
				if (   Output.size() >= 1
					&& Output.back() == '\n')
				Output.pop_back();
			}

			m_OutputWidget->SetContent(Output);
		});
	}
}

ShellWidget::~ShellWidget()
{
}

void ShellWidget::ProcessEvent(InputEvent & InputEvent)
{
	// TEST
	if (false == InputEvent.m_Handled)
	{
		if (InputEvent.HasType(InputEvent::EventType::BUTTON_EVENT))
		{
			printf("Shell : InputEvent, v cat %d\n", InputEvent.m_Pointer->GetVirtualCategory());

			if (Pointer::VirtualCategory::TYPING == InputEvent.m_Pointer->GetVirtualCategory())
			{
				auto ButtonId = InputEvent.m_InputId;
				bool Pressed = InputEvent.m_Buttons[0];		// TODO: Check if there are >1 buttons

				if (Pressed)
				{
					switch (ButtonId)
					{
					case 'R':
						if (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LSUPER)
							|| InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RSUPER))
						{
							m_ExecuteWidget->Action();

							InputEvent.m_Handled = true;
						}
						break;
					default:
						break;
					}
				}
			}
		}
	}
}
