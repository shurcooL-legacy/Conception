#include "../Main.h"

LiveFunctionWidget::LiveFunctionWidget(Vector2n Position, TypingModule & TypingModule)
	: CompositeWidget(Position, { std::shared_ptr<Widget>(m_SourceWidget = new TextFieldWidget(Vector2n(0, 0), TypingModule)),
								  std::shared_ptr<Widget>(m_OutputWidget = new TextFieldWidget(Vector2n(0, 0), TypingModule)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
	m_SourceWidget->m_OnChange = [&]()
	{
		m_OutputWidget->SetPosition(Vector2n(m_SourceWidget->GetDimensions().X() + 2, 0));
		m_OutputWidget->SetContent(m_SourceWidget->GetContent());
	};

	m_SourceWidget->m_OnChange();
}

LiveFunctionWidget::~LiveFunctionWidget()
{
}
