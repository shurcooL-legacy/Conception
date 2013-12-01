#include "../Main.h"

LiveProgramFileWidget::LiveProgramFileWidget(Vector2n Position, std::string Path, TypingModule & TypingModule, Project & Project)
  : FlowLayoutWidget(Position, {
		std::shared_ptr<Widget>(m_SourceFileWidget = new TextFileWidget(Vector2n::ZERO, Path, TypingModule)),
		std::shared_ptr<Widget>(m_ProgramWidget = new ProgramWidget(Vector2n::ZERO, TypingModule, Project, m_SourceFileWidget->m_TextFieldWidget))
	}, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) }),
	m_SourceWidget(m_SourceFileWidget->m_TextFieldWidget)
{
	m_SourceFileWidget->RemoveAllBehaviors();
	m_ProgramWidget->RemoveAllBehaviors();

	m_SourceWidget->m_GetAutocompletions = GetAutocompletions(m_SourceFileWidget->GetPath());

	// Add Dependee connector to the source widget (for connecting text fields that the source widget depends on)
	for (int i = 0; i < 3; ++i)
	{
		auto Dependee = new ConnectionWidget<TextFieldWidget>(Vector2n(-16 - 2, (16 + 2) * i));
		// TODO: Should do NotifyExternalChange() as long as I don't expect the contents of this thing to actually depend on the dependee...
		Dependee->m_OnChange = [=]() { m_SourceWidget->NotifyChange(); };
		m_SourceWidget->AddWidget(Dependee);
	}

	ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('R', PointerState::Modifiers::Super, [=]() { m_SourceWidget->NotifyChange(true); }, "Run/Refresh"));

	auto GofmtFile = [Path, &TypingModule]() {
		auto Shell = std::unique_ptr<ShellWidget>(new ShellWidget(Vector2n::ZERO, TypingModule));
		std::string Command = "goimports -w \'" + Path + "\'";
		Shell->m_CommandWidget->SetContent(Command);
		Shell->m_ExecuteWidget->GetAction()();

		std::cerr << "Doing: " << Shell->m_CommandWidget->GetContent() << endl;
		std::cerr << Shell->m_OutputWidget->GetContent() << endl;
	};
	ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('S', PointerState::Modifiers::Super, GofmtFile, "goimports"));
}

LiveProgramFileWidget::~LiveProgramFileWidget()
{
}
