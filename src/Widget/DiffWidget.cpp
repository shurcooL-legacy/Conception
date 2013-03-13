#include "../Main.h"

DiffWidget::DiffWidget(Vector2n Position, TypingModule & TypingModule, TextFieldWidget * Target1, TextFieldWidget * Target2)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(m_Source1Widget = new ConnectionWidget<TextFieldWidget>(Vector2n::ZERO, Target1)),
								   std::shared_ptr<Widget>(m_Source2Widget = new ConnectionWidget<TextFieldWidget>(Vector2n::ZERO, Target2)),
								   std::shared_ptr<Widget>(new LabelWidget(Vector2n::ZERO, std::string("diff"), LabelWidget::Background::Normal)),
								   std::shared_ptr<Widget>(m_OutputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)) },
								 { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) } )
{
	m_OutputWidget->m_GetLineHighlighting = GetLineHighlighting();

	auto OnChange = [=]()
	{
		if (m_Source1Widget->Target() && m_Source2Widget->Target())
		{
			auto Out = Diff(m_Source1Widget->Target()->GetContent(), m_Source2Widget->Target()->GetContent());

			SkipFirstLines(Out, 2);

			// Trim last newline, if there is one
			TrimLastNewline(Out);

			m_OutputWidget->SetContent(Out);
		}
	};

	m_Source1Widget->m_OnChange = OnChange;
	m_Source2Widget->m_OnChange = OnChange;

	OnChange();
}

DiffWidget::~DiffWidget()
{
}
