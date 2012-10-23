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

void DraggablePositionBehavior::ProcessManipulationStarted(const PointerState & PointerState)
{
	if (!m_Widget.HasTypingFocus())
	{
		auto ParentLocalPosition = m_Widget.GlobalToParent(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));

		m_Widget.ModifyGestureRecognizer().m_ManipulationOffset = m_Widget.GetPosition() - ParentLocalPosition;
	}
}

void DraggablePositionBehavior::ProcessManipulationUpdated(const PointerState & PointerState)
{
	if (!m_Widget.HasTypingFocus())
	{
		auto ParentLocalPosition = m_Widget.GlobalToParent(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));

		m_Widget.ModifyPosition() = m_Widget.GetGestureRecognizer().m_ManipulationOffset + ParentLocalPosition;
	}
}

void DraggablePositionBehavior::ProcessManipulationCompleted(const PointerState & PointerState)
{
}
