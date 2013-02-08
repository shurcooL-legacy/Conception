#include "../Main.h"

Widget::Widget(Vector2n Position, Vector2n Dimensions, std::vector<std::shared_ptr<Behavior>> Behaviors)
	: GestureHandler(),
	  m_Visible(true),
	  m_Position(Position),
	  m_Dimensions(Dimensions),
	  //m_HoverPointers()
	  m_GestureRecognizer(*this),
	  m_Behaviors(Behaviors)
{
	for (auto Behavior : m_Behaviors)
	{
		Behavior->SetupGestureRecognizer();
	}
}

Widget::~Widget()
{
}

void Widget::AddBehavior(Behavior * Behavior)
{
	m_Behaviors.push_back(std::shared_ptr<class Behavior>(Behavior));
	// TODO: ModifyGestureRecognizer().UpdateAccordingToBehaviors(m_Behaviors);
	Behavior->SetupGestureRecognizer();
}

void Widget::RemoveAllBehaviors()
{
	for (auto & Behavior : m_Behaviors)
	{
		Behavior->UnsetupGestureRecognizer();
	}

	m_Behaviors.clear();
	// TODO: ModifyGestureRecognizer().UpdateAccordingToBehaviors(m_Behaviors);
}

bool Widget::HasTypingFocus() const
{
#if 0
	return (GetGestureRecognizer().GetConnected().end() != GetGestureRecognizer().GetConnected().find(g_InputManager->m_TypingPointer.get()));
#else
	auto TypingPointerIterator = GetGestureRecognizer().GetConnected().find(g_InputManager->m_TypingPointer.get());
	if (GetGestureRecognizer().GetConnected().end() != TypingPointerIterator)
	{
		return (&GetGestureRecognizer() == (*TypingPointerIterator)->GetPointerMapping().GetHoverer());
	}
	else
		return false;
#endif
}

bool Widget::CheckHover() const
{
	//return !m_HoverPointers.empty();
	//return false;
	//return (!GetGestureRecognizer().GetConnected().empty());		// TEST

	for (auto & Pointer : GetGestureRecognizer().GetConnected())
	{
		if (&GetGestureRecognizer() == Pointer->GetPointerMapping().GetHoverer())
			return true;
	}

	return false;
}

bool Widget::CheckActive() const
{
	//return (this == WidgetManager.m_ActiveWidget);
	/*for (auto Pointer : m_HoverPointers)
	{
		//if (this == Pointer->GetWidgetMapping().GetActiveWidget())
		{
			return true;
		}
	}*/

	for (auto & Pointer : GetGestureRecognizer().GetConnected())
	{
		Vector2n GlobalPosition(Pointer->GetPointerState().GetAxisState(0).GetPosition(), Pointer->GetPointerState().GetAxisState(1).GetPosition());

		if (   true == Pointer->GetPointerState().GetButtonState(0)
			&& IsHit(GlobalToParent(GlobalPosition)))
		{
			return true;
		}
	}

	return false;
}

void Widget::ProcessCanvasUpdated()
{
	/*if (nullptr != m_GestureRecognizerTEST)
	{
		m_GestureRecognizerTEST->ProcessCanvasUpdated();
	}*/
	ModifyGestureRecognizer().ProcessCanvasUpdated();
}

const CompositeWidget * Widget::GetParent() const
{
	return m_Parent;
}

CompositeWidget * Widget::ModifyParent()
{
	return m_Parent;
}

void Widget::SetParent(CompositeWidget & Parent)
{
	m_Parent = &Parent;
}

MatchResult Widget::MatchEventQueue(InputEventQueue::FilteredQueue & UnreservedEvents)
{
	return m_GestureRecognizer.MatchEventQueue(UnreservedEvents);
}

// Returns true if there is any (even partial) hit
// Returns false if there is no hit whatsoever
bool Widget::HitTest(Vector2n ParentPosition, std::list<Widget *> * Hits) const
{
	auto Hit = IsHit(ParentPosition);

	if (Hit && nullptr != Hits)
	{
		Hits->push_back(const_cast<Widget *>(this));
	}

	return Hit;
}

void Widget::HitTest2(Vector2n ParentPosition, PointerMapping & Mapping) const
{
	auto Hit = IsHit(ParentPosition);

	if (Hit)
	{
		for (auto GestureType : m_GestureTypes)
		{
			;
		}
	}

	// TODO: Finish
	throw 0;
}

bool Widget::IsHit(const Vector2n ParentPosition) const
{
	if (!m_Visible)
		return false;

	bool Hit = (   ParentPosition.X() >= m_Position.X()
				&& ParentPosition.Y() >= m_Position.Y()
				&& ParentPosition.X() <  m_Position.X() + m_Dimensions.X()
				&& ParentPosition.Y() <  m_Position.Y() + m_Dimensions.Y());

	return Hit;
}

const Vector2n Widget::ParentToLocal(const Vector2n ParentPosition) const
{
	return ParentPosition - m_Position;
}

const Vector2n Widget::GlobalToParent(const Vector2n GlobalPosition) const
{
	Vector2n ParentPosition = GlobalPosition;

	if (nullptr != GetParent())
	{
		ParentPosition = GetParent()->GlobalToLocal(ParentPosition);
	}

	return ParentPosition;
}

const Vector2n Widget::GlobalToLocal(const Vector2n GlobalPosition) const
{
	/*Vector2d CanvasPositionDouble = GetParent()->GlobalToCanvas(GlobalPosition);
	Vector2n CanvasPosition(std::lround(CanvasPositionDouble.X()), std::lround(CanvasPositionDouble.Y()));		// TODO: Loss of accuracy? Fix it if needed.

	return CanvasPosition - m_Position;*/

	return ParentToLocal(GlobalToParent(GlobalPosition));
}

const Vector2n Widget::GetPosition() const
{
	return m_Position;
}

const Vector2n Widget::GetDimensions() const
{
	if (!m_Visible)
		return Vector2n::ZERO;

	return m_Dimensions;
}

void Widget::SetPosition(Vector2n Position)
{
	m_Position = Position;
}

void Widget::SetDimensions(Vector2n Dimensions)
{
	m_Dimensions = Dimensions;
}

Vector2n & Widget::ModifyPosition()
{
	return m_Position;
}

Vector2n & Widget::ModifyDimensions()
{
	return m_Dimensions;
}

/*void Widget::AddHoverPointer(Pointer * Pointer)
{
	m_HoverPointers.insert(Pointer);
}

void Widget::RemoveHoverPointer(Pointer * Pointer)
{
	m_HoverPointers.erase(Pointer);
}*/

void Widget::ProcessManipulationBegin(const PointerState & PointerState)
{
	for (auto & Behavior : m_Behaviors)
	{
		Behavior->ProcessManipulationBegin(PointerState);
	}
}

void Widget::ProcessManipulationUpdate(const PointerState & PointerState)
{
	for (auto & Behavior : m_Behaviors)
	{
		Behavior->ProcessManipulationUpdate(PointerState);
	}
}

void Widget::ProcessManipulationEnd(const PointerState & PointerState)
{
	for (auto & Behavior : m_Behaviors)
	{
		Behavior->ProcessManipulationEnd(PointerState);
	}
}
