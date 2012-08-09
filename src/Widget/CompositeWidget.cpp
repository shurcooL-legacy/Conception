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

/*
// HACK
// CONTINUE: Fix hack, it works for rendering but not input
auto CompositeParent = dynamic_cast<const CompositeWidget *>(GetParent());
if (nullptr != CompositeParent)
{
	return m_Position + CompositeParent->GetPosition();
}*/

bool CompositeWidget::HitTest(Vector2n ParentPosition, std::list<Widget *> * Hits) const
{
	//for (auto & Widget : m_Widgets)
	for (auto Widget = m_Widgets.rbegin(); m_Widgets.rend() != Widget; ++Widget)
	{
		auto Result = (*Widget)->HitTest(ParentToLocal(ParentPosition), Hits);
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
