#include "../Main.h"

TextFileWidget::TextFileWidget(Vector2n Position, std::string Path, TypingModule & TypingModule)
	: CompositeWidget(Position, { std::shared_ptr<Widget>(new LabelWidget(Vector2n(0, -lineHeight - 2), Path, LabelWidget::Background::Normal)),
								  std::shared_ptr<Widget>(m_TextFieldWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)) },
								{ std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) }),
	  m_Path(Path)
{
	m_TextFieldWidget->SetContent(FromFileToString(Path));
	m_TextFieldWidget->m_OnChange = [=]()
	{
		//PlayBeep();

		// Write to file
		WriteToFile(Path, m_TextFieldWidget->GetContent());

		// Call TextFileWidget's m_OnChange when the TextFieldWidget is modified
		if (nullptr != m_OnChange) {
			m_OnChange();
		}
	};
}

TextFileWidget::~TextFileWidget()
{
}

void TextFileWidget::ProcessTimePassed(const double TimePassed)
{
	// Check if the file has been changed externally, and if so, override this widget
	{
		auto NewContent = FromFileToString(m_Path);
		if (NewContent != m_TextFieldWidget->GetContent())
			m_TextFieldWidget->SetContent(NewContent);
	}

	CompositeWidget::ProcessTimePassed(TimePassed);
}

std::string TextFileWidget::GetPath() const
{
	return m_Path;
}
