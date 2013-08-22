#include "../Main.h"

RubyWidget::RubyWidget(Vector2n Position, TypingModule & TypingModule, TextFieldWidget * Target)
  : FlowLayoutWidget(Position, {
		std::shared_ptr<Widget>(m_SourceWidget = new ConnectionWidget<TextFieldWidget>(Vector2n::ZERO, Target)),
		std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("ruby"), LabelWidget::Background::Normal)),
		std::shared_ptr<Widget>(m_OutputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule))
	}, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
	m_SourceWidget->m_OnChange = [&]()
	{
		//PlayBeep();

		std::string InOut = "";

		if (nullptr != m_SourceWidget->Target())
		{
			InOut = m_SourceWidget->Target()->GetContent();

			InOut = WaitProcessWithStdinStdout("/usr/local/var/rbenv/shims/ruby", InOut);

			// Trim last newline, if there is one
			//TrimLastNewline(InOut);
		}

		m_OutputWidget->SetContent(InOut);
	};

	m_SourceWidget->m_OnChange();

	ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('R', PointerState::Modifiers::Super, m_SourceWidget->m_OnChange, "Run/Refresh"));
}

RubyWidget::~RubyWidget()
{
}
