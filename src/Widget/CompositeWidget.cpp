#include "../Main.h"

CompositeWidget::CompositeWidget(Vector2n Position, std::initializer_list<std::shared_ptr<Widget>> Widgets, std::vector<std::shared_ptr<Behavior>> Behaviors)
	: CompositeWidget(Position, Vector2n::ZERO, Widgets, Behaviors)
{}

CompositeWidget::CompositeWidget(Vector2n Position, Vector2n Dimensions, std::initializer_list<std::shared_ptr<Widget>> Widgets, std::vector<std::shared_ptr<Behavior>> Behaviors)
	: Widget(Position, Dimensions, Behaviors),
	  m_Widgets(Widgets)
{
	for (auto & Widget : GetWidgets())
	{
		Widget->SetParent(*this);
	}
}

CompositeWidget::~CompositeWidget()
{
}

void CompositeWidget::AddWidget(Widget * Widget)
{
	m_WidgetsToBeAdded.push(std::unique_ptr<class Widget>(Widget));
	Widget->SetParent(*this);
}

void CompositeWidget::RemoveWidget(Widget * Widget)
{
	// TODO: Finish
	// HACK: This removes all widgets, not just the specified one

	// For all connected pointers
	while (!Widget->ModifyGestureRecognizer().GetConnected().empty())
	{
		// Modify the pointer mapping of said pointer, and removed the widget which is being removed
		Widget->ModifyGestureRecognizer().GetConnected().begin().operator *()->ModifyPointerMapping().RemoveMapping(Widget->ModifyGestureRecognizer());
	}

	m_Widgets.clear();
}

void CompositeWidget::Render()
{
	glPushMatrix();
	glTranslated(GetPosition().X(), GetPosition().Y(), 0);
	{
		for (auto & Widget : GetWidgets())
		{
			Widget->Render();
		}
	}
	glPopMatrix();
}

MatchResult CompositeWidget::InnerMatchEventQueue(InputEventQueue::FilteredQueue & UnreservedEvents)
{
	for (auto & Widget : reverse(m_Widgets))
	{
		auto Match = Widget->MatchEventQueue(UnreservedEvents);

		if (Match.AnySuccess())
			return Match;
	}

	return MatchResult();
}

MatchResult CompositeWidget::MatchEventQueue(InputEventQueue::FilteredQueue & UnreservedEvents)
{
	MatchResult Match;

	// Prematching
	// TODO: Consider adding?

	// Child Widget matching
	if ((Match = InnerMatchEventQueue(UnreservedEvents)).AnySuccess())
	{
	}
	// Postmatching
	else if ((Match = Widget::MatchEventQueue(UnreservedEvents)).AnySuccess())
	{
	}

	return Match;
}

bool CompositeWidget::HitTest(Vector2n ParentPosition, std::list<Widget *> * Hits) const
{
#if 1
	for (auto & Widget : reverse(GetWidgets()))
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
#else
	bool HitInside = false;

	// HACK, TESTING: Swapped order of inner widgets and this widget for testing
	//>> FIX: Make clicking inside text area work...
	for (auto & Widget : reverse(GetWidgets()))
	{
		auto Result = Widget->HitTest(ParentToLocal(ParentPosition), nullptr);
#if DECISION_POINTER_MAPPING_CONTAINS_SINGLE_TOPMOST_WIDGET
		if (Result)
			return true;
#endif

		if (Result)
			HitInside = true;
	}

	if (Widget::HitTest(ParentPosition, Hits))
		//return true;
		HitInside = true;

	if (HitInside && nullptr != Hits)
	{
		Hits->push_back(const_cast<CompositeWidget *>(this));
		//return true;
	}

	for (auto & Widget : reverse(GetWidgets()))
	{
		auto Result = Widget->HitTest(ParentToLocal(ParentPosition), Hits);
#if DECISION_POINTER_MAPPING_CONTAINS_SINGLE_TOPMOST_WIDGET
		if (Result)
			return true;
#endif
	}

	if (HitInside)
		return true;

	return false;
#endif
}

const Vector2n CompositeWidget::GetDimensions() const
{
	auto Dimensions = Widget::GetDimensions();

	// TODO: Figure out if this is what I want to do... and make it proper, etc.
	for (auto & Widget : GetWidgets())
	{
		auto FarCorner = Widget->GetPosition() + Widget->GetDimensions();

		if (FarCorner.X() > Dimensions.X())
			Dimensions.X() = FarCorner.X();
		if (FarCorner.Y() > Dimensions.Y())
			Dimensions.Y() = FarCorner.Y();
	}

	return Dimensions;
}

void CompositeWidget::ProcessTimePassed(const double TimePassed)
{
	for (auto & Widget : GetWidgets())
	{
		Widget->ProcessTimePassed(TimePassed);
	}

	Widget::ProcessTimePassed(TimePassed);		// For DraggablePositionBehavior::ProcessTimePassed() to get called

	while (!m_WidgetsToBeAdded.empty())
	{
		m_Widgets.push_back(m_WidgetsToBeAdded.front());
		m_WidgetsToBeAdded.pop();
	}
}

// TODO: Think this through
bool CompositeWidget::IsHit(const Vector2n ParentPosition) const
{
	for (auto & Widget : reverse(m_Widgets))
	{
		if (Widget->IsHit(ParentToLocal(ParentPosition)))
			return true;
	}

	return Widget::IsHit(ParentPosition);
}
