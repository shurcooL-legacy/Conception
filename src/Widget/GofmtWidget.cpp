#include "../Main.h"

GofmtWidget::GofmtWidget(Vector2n Position, TypingModule & TypingModule, TextFieldWidget * Target)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(m_SourceWidget = new ConnectionWidget<TextFieldWidget>(Vector2n::ZERO, Target)),
								   std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("gofmt"), LabelWidget::Background::Normal)),
								   std::shared_ptr<Widget>(m_OutputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
	m_SourceWidget->m_OnChange = [&]()
	{
		//PlayBeep();

		std::string InOut = "";

		if (nullptr != m_SourceWidget->Target())
		{
			InOut = m_SourceWidget->Target()->GetContent();

			// gofmt
			auto Op = &Gofmt;
			Op(InOut);

			// Trim last newline, if there is one
			Op = &TrimLastNewline;
			Op(InOut);
		}

		m_OutputWidget->SetContent(InOut);
	};

	m_SourceWidget->m_OnChange();

	// TODO: Make this work in LiveGofmtWidget too (currently the Cmd+R event doesn't go from LiveGofmtWidget to this GofmtWidget)
	ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('R', PointerState::Modifiers::Super, m_SourceWidget->m_OnChange));
}

GofmtWidget::~GofmtWidget()
{
}
