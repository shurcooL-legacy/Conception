#include "../Main.h"

CompositeWidget::CompositeWidget(Vector2n Position, std::initializer_list<std::shared_ptr<Widget>> Widgets)
	: Widget(Position, Vector2n::ZERO),
	  m_Widgets(Widgets)
{
}

CompositeWidget::~CompositeWidget()
{
}

void CompositeWidget::Render()
{
	for (auto & Widget : m_Widgets)
	{
		Widget->Render();
	}
}

bool CompositeWidget::HitTest(Vector2n ParentPosition, std::list<Widget *> * Hits) const
{
	//for (auto & Widget : m_Widgets)
	for (auto Widget = m_Widgets.rbegin(); m_Widgets.rend() != Widget; ++Widget)
	{
		auto Result = (*Widget)->HitTest(ParentPosition, Hits);
#if DECISION_POINTER_MAPPING_CONTAINS_SINGLE_TOPMOST_WIDGET
		if (Result)
			return true;
#else
#	error Not implemented.
#endif
	}

	if (Widget::HitTest(ParentPosition, Hits))
		return true;

	return false;
}
