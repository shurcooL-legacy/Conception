#include "../Main.h"

ProgramWidget::ProgramWidget(Vector2n Position, TypingModule & TypingModule, Project & Project, TextFieldWidget * Target)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(m_SourceWidget = new ConnectionWidget<TextFieldWidget>(Vector2n::ZERO, Target)),
								   std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("go run"), LabelWidget::Background::Normal)),
								   std::shared_ptr<Widget>(m_OutputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) }),
	  m_Project(Project)
{
	{
		m_SourceWidget->m_OnChange = [=, &Project]()
		{
			//PlayBeep();

			Project.m_ProcessEndedTime = glfwGetTime();
			Project.m_BackgroundState = 0;

			// Kill child processes
			if (0 != Project.m_LastPid)
			{
				//std::cout << "Sending kill to last child pid " << Project.m_LastPid << ".\n";
				//auto Result = kill(0, SIGTERM);
				auto Result = killpg(Project.m_LastPid, SIGKILL);
				//waitpid(m_LastPid, NULL, 0);

				if (0 != Result) {
					std::cerr << "Error: kill() failed with return " << Result << ", errno " << errno << ".\n";
					//throw 0;
				}
			}

			//std::cout << "Closing " << Project.m_PipeFd[0] << " and " << Project.m_PipeFd[1] << "; ";
			close(Project.m_PipeFd[0]);		// Close the read end of the pipe in the parent
			Project.m_PipeFd[0] = Project.m_PipeFd[1] = -1;

			if (!m_SourceWidget->m_LiveToggle->GetState())
			{
				//OutputWidget.m_Visible = false;
				return;
			}
			else
			{
				//OutputWidget.m_Visible = true;
			}

			std::string Content = "";
			if (nullptr != m_SourceWidget->Target()) {
				Content = m_SourceWidget->Target()->GetContent();
			}
			Project.GenerateProgram(Content);

			g_OutputWidget = m_OutputWidget;		// HACK
			//m_OutputWidget->SetContent("");
			Project.m_ProcessStartedTime = glfwGetTime();
			Project.m_ExpiredOutput = true;
			Project.m_BackgroundState = 1;
		};

		// If there's already a Target for source, then refresh ourselves
		m_SourceWidget->m_OnChange();
	}

	// TODO: Make this work in LiveProgramWidget too (currently the Cmd+R event doesn't go from LiveProgramWidget to this ProgramWidget)
	ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('R', PointerState::Modifiers::Super, m_SourceWidget->m_OnChange));
}

ProgramWidget::~ProgramWidget()
{
}

void ProgramWidget::SetTarget(TextFieldWidget * Target)
{
	m_SourceWidget->SetTarget(Target);
}

void ProgramWidget::ProcessTimePassed(const double TimePassed)
{
	if (m_SourceWidget->m_LiveToggle->GetState())
		m_OutputWidget->m_Foreground = Color::BLACK;
	else
		m_OutputWidget->m_Foreground = Color(0.5, 0.5, 0.5);

	// If we have some for us
	if (m_OutputWidget == g_OutputWidget)
	{
		// Get text from stdout
		if (-1 != m_Project.m_PipeFd[0])
		{
			char buffer[1024];
			ssize_t n;
			while (0 != (n = read(m_Project.m_PipeFd[0], buffer, sizeof(buffer))))
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

						std::cerr << "Error: Reading from pipe " << m_Project.m_PipeFd[0] << " failed with errno " << errno << ".\n";
						break;
					}
				}
				else
				{
					std::string str(buffer, n);
					if (m_Project.m_ExpiredOutput)
					{
						m_OutputWidget->SetContent(str);
						m_Project.m_ExpiredOutput = false;
					}
					else
					{
						m_OutputWidget->AppendContent(str);
					}
				}
			}

			// If the output is still expired after a second since process started, just clear the output
			if (glfwGetTime() >= m_Project.m_ProcessStartedTime + 1.0)
			{
				if (m_Project.m_ExpiredOutput)
				{
					m_OutputWidget->SetContent("");
					m_Project.m_ExpiredOutput = false;
				}
			}
		}
	}
}
