#include "../Main.h"

TextFileWidget::TextFileWidget(Vector2n Position, std::string Path, TypingModule & TypingModule)
	: CompositeWidget(Position, { std::shared_ptr<Widget>(new LabelWidget(Vector2n(0, -lineHeight - 2), Path, LabelWidget::Background::Normal)), std::shared_ptr<Widget>(m_TextFieldWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
	m_TextFieldWidget->SetContent(FromFileToString(Path));
	m_TextFieldWidget->m_OnChange = [=]()
	{
		//PlayBeep();

		// Write to file
		WriteToFile(Path, m_TextFieldWidget->GetContent());
	};
}

TextFileWidget::~TextFileWidget()
{
}
