#include "../Main.h"

GoForcedUseWidget::GoForcedUseWidget(Vector2n Position, TypingModule & TypingModule, TextFieldWidget * Target)
  : FlowLayoutWidget(Position, {
		std::shared_ptr<Widget>(m_SourceWidget = new ConnectionWidget<TextFieldWidget>(Vector2n::ZERO, Target)),
		std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("go Forced Use"), LabelWidget::Background::Normal)),
		std::shared_ptr<Widget>(m_OutputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule))
	}, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) }),
	m_ShellWidget(new ShellWidget(Vector2n::ZERO, TypingModule))
{
	m_ShellWidget->m_CommandWidget->SetContent("cat | goe fmt 'gist.github.com/4727543.git' 'gist.github.com/5498057.git' 'Println(GetForcedUseFromImport(ReadAllStdin()))' | head -n 1");

	m_SourceWidget->m_OnChange = [&]()
	{
		std::string Out = "";

		if (   nullptr != m_SourceWidget->Target()
			&& m_SourceWidget->Target()->GetContent().length() > 0)
		{
			m_ShellWidget->m_StdInWidget->SetContent(m_SourceWidget->Target()->GetContent());
			m_ShellWidget->m_ExecuteWidget->GetAction()();
			Out = m_ShellWidget->m_OutputWidget->GetContent();

			// Trim last newline, if there is one
			TrimLastNewline(Out);
		}

		m_OutputWidget->SetContent(Out);
	};

	m_SourceWidget->m_OnChange();

	ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('R', PointerState::Modifiers::Super, m_SourceWidget->m_OnChange, "Run/Refresh"));
}

GoForcedUseWidget::~GoForcedUseWidget()
{
}
