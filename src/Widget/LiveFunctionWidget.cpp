#include "../Main.h"

LiveFunctionWidget::LiveFunctionWidget(Vector2n Position, TypingModule & TypingModule, Project & Project)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(m_InputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)),
								   std::shared_ptr<Widget>(m_SourceWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)),
								   std::shared_ptr<Widget>(m_OutputWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) }),
	  m_Project(Project)
{
	/*m_SourceWidget->m_OnChange = [&]()
	{
		m_OutputWidget->SetContent(m_InputWidget->GetContent() + m_SourceWidget->GetContent());
	};
	m_InputWidget->m_OnChange = m_SourceWidget->m_OnChange;

	m_SourceWidget->m_OnChange();*/
}

LiveFunctionWidget::~LiveFunctionWidget()
{
}
