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
	// DEBUG: Not sure if still need to do this now that widget is removed after event queue processing, but maybe still needed to rid of persistent pointers
	// For all connected pointers
	// DEBUG: Disabled because it caused intermittent crashing of FolderListingWidget...
	// DEBUG: Re-enabled because fixed root cause of crashing, it was this widget being deleted more than once because I wasn't resetting m_Child back to nullptr after deleting
	while (!ModifyGestureRecognizer().GetConnected().empty())
	{
		// Modify the pointer mapping of said pointer, and removed the widget which is being removed
		ModifyGestureRecognizer().GetConnected().begin().operator *()->ModifyPointerMapping().RemoveMapping(ModifyGestureRecognizer());
	}
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
	return HasAnyTypingFocus();
#else
	return HasTopmostTypingFocus();
#endif
}

// DEBUG: Not sure if I should make such a distinction as "topmost" widget, or better to avoid it, or have a "request/accept focus" system so then it's topmost-widget-that-requested-focus
bool Widget::HasTopmostTypingFocus() const
{
	auto TypingPointerIterator = GetGestureRecognizer().GetConnected().find(g_InputManager->m_TypingPointer.get());
	if (GetGestureRecognizer().GetConnected().end() != TypingPointerIterator)
	{
		return (&GetGestureRecognizer() == (*TypingPointerIterator)->GetPointerMapping().GetHoverer());
	}
	else
		return false;
}

bool Widget::HasAnyTypingFocus() const
{
	return (GetGestureRecognizer().GetConnected().end() != GetGestureRecognizer().GetConnected().find(g_InputManager->m_TypingPointer.get()));
}

bool Widget::CheckHover() const
{
	// DEBUG: Why only loop over the connected pointers? If I'm using m_CurrentPointerMappingTEST, then it may not be connected at all... Doesn't make sense, I should fix this inconsistency
	for (auto & Pointer : GetGestureRecognizer().GetConnected())
	{
		if (&GetGestureRecognizer() == Pointer->m_CurrentPointerMappingTEST.GetHoverer())
		{
			return true;
		}
	}

	return false;
}

bool Widget::CheckActive() const
{
	for (auto & Pointer : GetGestureRecognizer().GetConnected())
	{
		if (   &GetGestureRecognizer() == Pointer->GetPointerMapping().GetHoverer()		// Pointer was hovering this very widged when pointer went active
			&& true == Pointer->GetPointerState().GetButtonState(0))					// Pointer button 0 is down
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

	bool Hit = (   ParentPosition.X() >= GetPosition().X()
				&& ParentPosition.Y() >= GetPosition().Y()
				&& ParentPosition.X() <  GetPosition().X() + GetDimensions().X()
				&& ParentPosition.Y() <  GetPosition().Y() + GetDimensions().Y());

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

const Vector2n Widget::LocalToParent(const Vector2n LocalPosition) const
{
	auto Delta = Widget::ParentToLocal(LocalPosition) - LocalPosition;
	return LocalPosition - Delta;
}
const Vector2n Widget::ParentToGlobal(const Vector2n ParentPosition) const
{
	Vector2n GlobalPosition = ParentPosition;

	if (nullptr != GetParent())
	{
		GlobalPosition = GetParent()->LocalToGlobal(ParentPosition);
	}

	return GlobalPosition;
}
const Vector2n Widget::LocalToGlobal(const Vector2n LocalPosition) const
{
	return ParentToGlobal(LocalToParent(LocalPosition));
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

void Widget::ProcessManipulationBegin(const InputEvent & InputEvent)
{
	for (auto & Behavior : m_Behaviors)
	{
		Behavior->ProcessManipulationBegin(InputEvent);
	}
}

void Widget::ProcessManipulationUpdate(const InputEvent & InputEvent)
{
	for (auto & Behavior : m_Behaviors)
	{
		Behavior->ProcessManipulationUpdate(InputEvent);
	}
}

void Widget::ProcessManipulationEnd(const InputEvent & InputEvent)
{
	for (auto & Behavior : m_Behaviors)
	{
		Behavior->ProcessManipulationEnd(InputEvent);
	}
}

void Widget::ProcessTimePassed(const double TimePassed)
{
	// For DraggablePositionBehavior::ProcessTimePassed() to get called
	for (auto & Behavior : m_Behaviors)
	{
		Behavior->ProcessTimePassed(TimePassed);
	}
}
