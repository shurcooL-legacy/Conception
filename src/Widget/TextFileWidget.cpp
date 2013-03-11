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

	// TEST: Line Gutters
#if 0
	//if ("./Gen/5086673/gistfile1.go" == Path)
	m_TextFieldWidget->m_GetLineGutters = [=](uint32 LineNumber) -> std::string
	{
#if 0
		std::string x = "."; Ls(x);
		return std::to_string(LineNumber + 1);
#endif
		// HACK: Pass file folder and name info
		auto Separator = Path.find_last_of('/');
		if (0 == Separator) throw 0;		// TODO: Handle "/rooted_paths" properly
		std::string Folder = std::string::npos != Separator ? Path.substr(0, Separator) : "./";
		std::string Filename = std::string::npos != Separator ? Path.substr(Separator + 1) : Path;
		if (0 == LineNumber)
			return Folder;
		else if (1 == LineNumber)
			return Filename;
		else
			throw 0;
	};
#endif
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
