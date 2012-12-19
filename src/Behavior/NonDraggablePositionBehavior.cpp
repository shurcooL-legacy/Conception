#include "../Main.h"

NonDraggablePositionBehavior::NonDraggablePositionBehavior(Widget & Widget)
	: Behavior(Widget)
{
}

NonDraggablePositionBehavior::~NonDraggablePositionBehavior()
{
}

void NonDraggablePositionBehavior::SetupGestureRecognizer()
{
	m_Widget.ModifyGestureRecognizer().m_RecognizeManipulationTranslate = true;
}
