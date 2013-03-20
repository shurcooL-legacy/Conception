#include "../Main.h"

TextFileWidget::TextFileWidget(Vector2n Position, std::string Path, TypingModule & TypingModule)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(new LabelWidget(Vector2n(0, -lineHeight - 2), Path, LabelWidget::Background::Normal)),
								   std::shared_ptr<Widget>(m_TextFieldWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)) },
								 { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) }, FlowLayoutWidget::LayoutType::Vertical),
	  m_Path(Path)
{
	m_TextFieldWidget->SetContent(FromFileToString(Path));
	m_OnChange = [=]()		// Saving takes place in TextFileWidget when it gets its NotifyChange() from the contained TextFieldWidget
	{
		//PlayBeep();
		//printf("Saving '%s'.\n", Path.c_str());

		// Write to file
		WriteToFile(Path, m_TextFieldWidget->GetContent());
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
		std::string Folder = ParsePath(Path, 0);
		std::string Filename = ParsePath(Path, 1);
		if (0 == LineNumber)
			return Folder;
		else if (1 == LineNumber)
			return Filename;
		else
			throw 0;
	};
#endif

	if (IsFileTrackedByGit(Path)) {
		auto GitDiff = new GitDiffWidget(Vector2n::ZERO, TypingModule, this);
		GitDiff->RemoveAllBehaviors();
		AddWidget(GitDiff);
	}
}

TextFileWidget::~TextFileWidget()
{
}

void TextFileWidget::NotifyChange(bool OverrideLiveToggle)
{
	if (nullptr != m_OnChange) {
		m_OnChange();
	}
	NotifyExternalChange(OverrideLiveToggle);
}

void TextFileWidget::NotifyExternalChange(bool OverrideLiveToggle)
{
	for (auto ConnectionWidget : GetConnected()) {
		ConnectionWidget->NotifyChange(OverrideLiveToggle);
	}
}

void TextFileWidget::ProcessTimePassed(const double TimePassed)
{
	// Check if the file has been changed externally, and if so, override this widget
	{
		auto NewContent = FromFileToString(m_Path);
		if (NewContent != m_TextFieldWidget->GetContent())
			// TODO: Make it so that a 'save' is not called in turn...
			m_TextFieldWidget->SetContent(NewContent);
	}

	CompositeWidget::ProcessTimePassed(TimePassed);
}

std::string TextFileWidget::GetPath() const
{
	return m_Path;
}
