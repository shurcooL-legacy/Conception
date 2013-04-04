#include "../Main.h"

TextFileWidget::TextFileWidget(Vector2n Position, std::string Path, TypingModule & TypingModule)
	: FlowLayoutWidget(Position, {
		std::shared_ptr<Widget>(new FlowLayoutWidget(Vector2n::ZERO, {
			std::shared_ptr<Widget>(m_FileMinimizeToggle = new ToggleWidget(Vector2n::ZERO, Vector2n(12, 12), [](bool State) { if (!State) g_InputManager->RequestTypingPointer(*static_cast<GestureRecognizer *>(nullptr)); }, true)),
			std::shared_ptr<Widget>(new LabelWidget(Vector2n(0, -lineHeight - 2), Path, LabelWidget::Background::Normal))
		}, {})),
		std::shared_ptr<Widget>(m_TextFieldWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule))
	}, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) }, FlowLayoutWidget::LayoutType::Vertical),
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

	const std::string Folder = ParsePath(Path, 0);
	const std::string Filename = ParsePath(Path, 1);

	auto CopyPath = [this, &TypingModule]() {
#if DECISION_USE_CLIPBOARD_INSTEAD_OF_TypingModule
		glfwSetClipboardString(this->m_Path);
#else
		TypingModule.SetString(this->m_Path);
#endif
	};
	ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('I', PointerState::Modifiers::Super, CopyPath, "Copy Path"));

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

		auto GitCommit = new ButtonWidget(Vector2n(-160, -350), [=]() {
				auto Shell = std::unique_ptr<ShellWidget>(new ShellWidget(Vector2n::ZERO, *static_cast<class TypingModule *>(nullptr)));
				std::string Command = "cd \'" + Folder + "\'\ngit commit --allow-empty-message -m '' -- \'" + Filename + "\'";
				Command += "\ngit push origin master";
				Shell->m_CommandWidget->SetContent(Command);
				Shell->m_ExecuteWidget->GetAction()();
				this->NotifyExternalChange();		// Do this to triger potential GitDiffWidget, GitStatusWidget, etc.

				//std::cerr << "Commit & Push: '" << Folder << "' folder and '" << Filename << "' file.\n";
				std::cerr << Shell->m_OutputWidget->GetContent() << endl;
			},
			"Commit & Push");
		AddWidget(GitCommit);
	}

	m_TextFieldWidget->m_MinimizeToggle = m_FileMinimizeToggle;
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

	FlowLayoutWidget::ProcessTimePassed(TimePassed);
}

std::string TextFileWidget::GetPath() const
{
	return m_Path;
}
