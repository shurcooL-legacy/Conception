#include "../Main.h"

CompositeWidget::CompositeWidget(Vector2n Position, std::initializer_list<std::shared_ptr<Widget>> Widgets)
	: Widget(Position, Vector2n::ZERO),
	  m_Widgets(Widgets)
{
	for (auto & Widget : m_Widgets)
	{
		Widget->SetParent(*this);
	}
}

CompositeWidget::~CompositeWidget()
{
}

void CompositeWidget::Render()
{
	glPushMatrix();
	glTranslated(GetPosition().X(), GetPosition().Y(), 0);
	{
		for (auto & Widget : m_Widgets)
		{
			Widget->Render();
		}
	}
	glPopMatrix();
}

bool CompositeWidget::HitTest(Vector2n ParentPosition, std::list<Widget *> * Hits) const
{
	for (auto & Widget : reverse(m_Widgets))
	{
		auto Result = Widget->HitTest(ParentToLocal(ParentPosition), Hits);
#if DECISION_POINTER_MAPPING_CONTAINS_SINGLE_TOPMOST_WIDGET
		if (Result)
			return true;
#endif
	}

	if (Widget::HitTest(ParentPosition, Hits))
		return true;

	return false;
}
