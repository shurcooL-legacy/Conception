#include "../Main.h"

TextFileWidget::TextFileWidget(Vector2n Position, std::string Path, TypingModule & TypingModule)
	: CompositeWidget(Position, { std::shared_ptr<Widget>(m_TextFieldWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
	m_TextFieldWidget->SetContent(FromFileToString(Path));
	m_TextFieldWidget->m_OnChange = [=]() {
		// Write to file
		{
			PlayBeep();

			//std::cout << "Writing the following to '" << Path << "':\n" << m_TextFieldWidget->GetContent() << endl;
			std::ofstream OutFile(Path);
			OutFile << m_TextFieldWidget->GetContent();
		}
	};
}

TextFileWidget::~TextFileWidget()
{
}
