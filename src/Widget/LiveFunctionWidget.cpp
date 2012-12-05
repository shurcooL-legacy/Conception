#include "../Main.h"

LiveFunctionWidget::LiveFunctionWidget(Vector2n Position, TypingModule & TypingModule, Project & Project)
	: CompositeWidget(Position, { std::shared_ptr<Widget>(m_SourceWidget = new TextFieldWidget(Vector2n(0, 0), TypingModule)),
								  std::shared_ptr<Widget>(m_OutputWidget = new TextFieldWidget(Vector2n(0, 0), TypingModule)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) }),
	  m_Project(Project)
{
#if 0
	m_SourceWidget->m_OnChange = [&]()
	{
		m_OutputWidget->SetPosition(Vector2n(m_SourceWidget->GetDimensions().X() + 2, 0));
		m_OutputWidget->SetContent(m_SourceWidget->GetContent());
	};
#else
	m_Project.SetSourceOnChange(*m_SourceWidget, *m_OutputWidget, nullptr, nullptr);
#endif

	m_SourceWidget->m_OnChange();
}

LiveFunctionWidget::~LiveFunctionWidget()
{
}
