#include "../Main.h"

DraggablePositionBehavior::DraggablePositionBehavior(Widget & Widget)
	: Behavior(Widget)
{
}

DraggablePositionBehavior::~DraggablePositionBehavior()
{
}

void DraggablePositionBehavior::SetupGestureRecognizer()
{
	m_Widget.ModifyGestureRecognizer().m_RecognizeManipulationTranslate = true;
}

void DraggablePositionBehavior::UnsetupGestureRecognizer()
{
	// TODO: This is wrong, I shouldn't change state... instead it should be calculated based on a function call?
	m_Widget.ModifyGestureRecognizer().m_RecognizeManipulationTranslate = false;
}

void DraggablePositionBehavior::ProcessManipulationBegin(const PointerState & PointerState)
{
	if (!m_Widget.HasTypingFocus())
	{
		auto ParentLocalPosition = m_Widget.GlobalToParent(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));

		m_Widget.ModifyGestureRecognizer().m_ManipulationOffset = m_Widget.GetPosition() - ParentLocalPosition;
	}
}

void DraggablePositionBehavior::ProcessManipulationUpdate(const PointerState & PointerState)
{
	if (!m_Widget.HasTypingFocus())
	{
		auto ParentLocalPosition = m_Widget.GlobalToParent(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));

		m_Widget.ModifyPosition() = m_Widget.GetGestureRecognizer().m_ManipulationOffset + ParentLocalPosition;
	}
}

void DraggablePositionBehavior::ProcessManipulationEnd(const PointerState & PointerState)
{
}

void DraggablePositionBehavior::ProcessTimePassed(const double TimePassed)
{
	// TODO: Merge with the automatic logic for setup/unsetup GR at the top... Instead of setting this explicitly here, it should get "calculated" where required...
	m_Widget.ModifyGestureRecognizer().m_RecognizeManipulationTranslate = !m_Widget.HasTypingFocus();
}
