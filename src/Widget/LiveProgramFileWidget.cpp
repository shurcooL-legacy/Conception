#include "../Main.h"

LiveProgramFileWidget::LiveProgramFileWidget(Vector2n Position, std::string Path, TypingModule & TypingModule, Project & Project)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(m_SourceFileWidget = new TextFileWidget(Vector2n::ZERO, Path, TypingModule)),
								   std::shared_ptr<Widget>(m_ProgramWidget = new ProgramWidget(Vector2n::ZERO, TypingModule, Project, m_SourceFileWidget->m_TextFieldWidget)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) }),
	  m_SourceWidget(m_SourceFileWidget->m_TextFieldWidget)
{
	m_SourceFileWidget->RemoveAllBehaviors();
	m_ProgramWidget->RemoveAllBehaviors();

	m_SourceWidget->m_GetAutocompletions = GetAutocompletions(m_SourceFileWidget->GetPath());

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

	ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('R', PointerState::Modifiers::Super, [=]() { m_SourceWidget->NotifyChange(true); } ));
}

LiveProgramFileWidget::~LiveProgramFileWidget()
{
}
