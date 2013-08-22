#include "../Main.h"

LiveRubyWidget::LiveRubyWidget(Vector2n Position, TypingModule & TypingModule)
  : FlowLayoutWidget(Position, {
		std::shared_ptr<Widget>(m_SourceWidget = new TextFieldWidget(Vector2n::ZERO, TypingModule)),
		std::shared_ptr<Widget>(m_RubyWidget = new RubyWidget(Vector2n::ZERO, TypingModule, m_SourceWidget))
	}, { std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this)) })
{
	m_RubyWidget->RemoveAllBehaviors();
}

LiveRubyWidget::~LiveRubyWidget()
{
}
