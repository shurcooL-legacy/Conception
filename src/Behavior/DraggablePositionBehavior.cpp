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

	// TEST: Close shortcut (assume if the widget is draggable, you might as well make it closeable)
	{
		auto Close = [this]() {
			if (nullptr != this->m_Widget.GetParent())
				this->m_Widget.ModifyParent()->RemoveWidget(&this->m_Widget);
		};
		m_Widget.ModifyGestureRecognizer().AddShortcut(GestureRecognizer::ShortcutEntry('W', PointerState::Modifiers::Super, Close, "Close"));
	}
}

void DraggablePositionBehavior::UnsetupGestureRecognizer()
{
	// TODO: This is wrong, I shouldn't change state... instead it should be calculated based on a function call?
	m_Widget.ModifyGestureRecognizer().m_RecognizeManipulationTranslate = false;

	// TODO: Unify this together with SetupGestureRecognizer, reduce duplication
	m_Widget.ModifyGestureRecognizer().RemoveShortcut(GestureRecognizer::ShortcutEntry::Key('W', PointerState::Modifiers::Super));
}

void DraggablePositionBehavior::ProcessManipulationBegin(const InputEvent & InputEvent)
{
	const PointerState & PointerState = InputEvent.m_PostEventState;

	// Check that either the widget is not selected, or the mouse is not hovering over the top-most typing pointer-active widget
	if (   !m_Widget.HasAnyTypingFocus()
		|| !InputEvent.m_Pointer->GetPointerMapping().ContainsMapping(g_InputManager->m_TypingPointer->GetPointerMapping().GetHoverer()))
	{
		auto ParentLocalPosition = m_Widget.GlobalToParent(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));

		m_Widget.ModifyGestureRecognizer().m_ManipulationOffset = m_Widget.GetPosition() - ParentLocalPosition;
	}
}

void DraggablePositionBehavior::ProcessManipulationUpdate(const InputEvent & InputEvent)
{
	const PointerState & PointerState = InputEvent.m_PostEventState;

	// Check that either the widget is not selected, or the mouse is not hovering over the top-most typing pointer-active widget
	if (   !m_Widget.HasAnyTypingFocus()
		|| !InputEvent.m_Pointer->GetPointerMapping().ContainsMapping(g_InputManager->m_TypingPointer->GetPointerMapping().GetHoverer()))
	{
		auto ParentLocalPosition = m_Widget.GlobalToParent(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));

		m_Widget.ModifyPosition() = m_Widget.GetGestureRecognizer().m_ManipulationOffset + ParentLocalPosition;
	}
}

void DraggablePositionBehavior::ProcessManipulationEnd(const InputEvent & InputEvent)
{
}

void DraggablePositionBehavior::ProcessTimePassed(const double TimePassed)
{
	// TODO: Merge with the automatic logic for setup/unsetup GR at the top... Instead of setting this explicitly here, it should get "calculated" where required...
	m_Widget.ModifyGestureRecognizer().m_RecognizeManipulationTranslate = !m_Widget.HasTypingFocus();
}
