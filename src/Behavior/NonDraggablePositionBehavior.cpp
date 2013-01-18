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

void NonDraggablePositionBehavior::UnsetupGestureRecognizer()
{
	// TODO: This is wrong, I shouldn't change state... instead it should be calculated based on a function call?
	m_Widget.ModifyGestureRecognizer().m_RecognizeManipulationTranslate = false;
}
