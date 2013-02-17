#include "../Main.h"

LiveGofmtWidget::LiveGofmtWidget(Vector2n Position, TypingModule & TypingModule)
	: FlowLayoutWidget(Position, { std::shared_ptr<Widget>(m_SourceWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)),
								   std::shared_ptr<Widget>(m_GofmtWidget = new GofmtWidget(Vector2n::ZERO, TypingModule, m_SourceWidget)) }, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
	m_GofmtWidget->RemoveAllBehaviors();
}

LiveGofmtWidget::~LiveGofmtWidget()
{
}
