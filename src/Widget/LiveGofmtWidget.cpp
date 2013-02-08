#include "../Main.h"

LiveGofmtWidget::LiveGofmtWidget(Vector2n Position, TypingModule & TypingModule, Project & Project)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(m_SourceWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)),
								   std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("gofmt"), LabelWidget::Background::Normal)),
								   std::shared_ptr<Widget>(m_OutputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
	{
		m_SourceWidget->m_OnChange = [&]()
		{
			std::string InOut = m_SourceWidget->GetContent();

			// gofmt
			auto Op = &Gofmt;
			Op(InOut);

			// Trim last newline, if there is one
			Op = &TrimLastNewline;
			Op(InOut);

			m_OutputWidget->SetContent(InOut);
		};
	}

	m_SourceWidget->m_OnChange();
}

LiveGofmtWidget::~LiveGofmtWidget()
{
}
