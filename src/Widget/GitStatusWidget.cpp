#include "../Main.h"

GitStatusWidget::GitStatusWidget(Vector2n Position, TypingModule & TypingModule, TextFileWidget * Target)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(m_SourceWidget = new ConnectionWidget<TextFileWidget>(Vector2n::ZERO, Target)),
								   std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("git status"), LabelWidget::Background::Normal)),
								   std::shared_ptr<Widget>(m_OutputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
	m_SourceWidget->m_OnChange = [&]()
	{
		std::string InOut = "";

		if (nullptr != m_SourceWidget->Target())
		{
			std::string Folder = ParsePath(m_SourceWidget->Target()->GetPath(), 0);

			auto Shell = std::unique_ptr<ShellWidget>(new ShellWidget(Vector2n::ZERO, TypingModule));
			std::string Command = "cd " + Folder + "\ngit status";
			Shell->m_CommandWidget->SetContent(Command);
			Shell->m_ExecuteWidget->GetAction()();
			InOut = Shell->m_OutputWidget->GetContent();

			TrimLastNewline(InOut);
		}

		m_OutputWidget->SetContent(InOut);
	};

	m_SourceWidget->m_OnChange();

	ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('R', PointerState::Modifiers::Super, m_SourceWidget->m_OnChange));
}

GitStatusWidget::~GitStatusWidget()
{
}
