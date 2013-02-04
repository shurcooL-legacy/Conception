#pragma once
#ifndef __GestureRecognizer_H__
#define __GestureRecognizer_H__

struct MatchResult
{
	uint8												Status;		// 0 - Impossible match, 1 = Potential match, 2 = Successful match
	InputEventQueue::FilteredQueue::const_iterator		End;
	InputEventQueue::FilteredQueue						Events;

	MatchResult() : MatchResult(0) {}
	MatchResult(uint8 Status) : Status(Status), End(), Events() {}
	//MatchResult(InputEventQueue::FilteredQueue & Events) : Status(2), End(), Events(Events) {}
	MatchResult(InputEventQueue::FilteredQueue::const_iterator End) : Status(2), End(End), Events() {}
	MatchResult(InputEventQueue::FilteredQueue::const_iterator End, InputEventQueue::FilteredQueue & Events) : Status(2), End(End), Events(Events) {}
	MatchResult(uint8 Status, InputEventQueue::FilteredQueue::const_iterator End, InputEventQueue::FilteredQueue & Events) : Status(Status), End(End), Events(Events) {}

	// Returns true if Status is non-zero
	// This is needed for `if (MatchResult)` statements
	bool AnySuccess() const { return 0 != Status; }
};

template <Pointer::VirtualCategory PointerVirtualCategory, Input::InputId ButtonId, bool ButtonState>
bool IsPointerButtonEvent(const InputEvent & InputEvent)
{
	return IsPointerButtonEvent<PointerVirtualCategory, ButtonState>(InputEvent, ButtonId);
}

template <Pointer::VirtualCategory PointerVirtualCategory, bool ButtonState>
bool IsPointerButtonEvent(const InputEvent & InputEvent, Input::InputId ButtonId)
{
	return (   nullptr != InputEvent.m_Pointer
			&& PointerVirtualCategory == InputEvent.m_Pointer->GetVirtualCategory()
			&& InputEvent.HasType(InputEvent::EventType::BUTTON_EVENT)
			&& ButtonId == InputEvent.m_InputId
			&& ButtonState == InputEvent.m_Buttons[0]
			&& !ButtonState == InputEvent.m_PreEventState.GetButtonState(ButtonId));
}

template <Input::InputId InputId>
bool IsPointerPointingMoveEvent(const InputEvent & InputEvent)
{
	return (   nullptr != InputEvent.m_Pointer
			&& Pointer::VirtualCategory::POINTING == InputEvent.m_Pointer->GetVirtualCategory()
			&& (   InputEvent.HasType(InputEvent::EventType::AXIS_EVENT)
				|| InputEvent.HasType(InputEvent::EventType::CANVAS_MOVED_TEST))
			&& InputId == InputEvent.m_InputId);
}

bool IsPointerPointingDeactivationEvent(const InputEvent & InputEvent);

uint8 MatchDoubleTap(const InputEventQueue::Queue & Queue, InputEventQueue::Queue::const_iterator & InputEventIterator);

uint8 MatchTap(const InputEventQueue::Queue & Queue, InputEventQueue::Queue::const_iterator & InputEventIterator);

MatchResult MatchDown(const InputEventQueue::FilteredQueue & Queue, InputEventQueue::FilteredQueue::const_iterator InputEventIterator);

MatchResult MatchUp(const InputEventQueue::FilteredQueue & Queue, InputEventQueue::FilteredQueue::const_iterator InputEventIterator);

MatchResult MatchManipulationBegin(const InputEventQueue::FilteredQueue & Queue, InputEventQueue::FilteredQueue::const_iterator InputEventIterator, bool InManipulationTEST, bool HitTEST);
MatchResult MatchManipulationUpdate(const InputEventQueue::FilteredQueue & Queue, InputEventQueue::FilteredQueue::const_iterator InputEventIterator, bool InManipulationTEST);
MatchResult MatchManipulationEnd(const InputEventQueue::FilteredQueue & Queue, InputEventQueue::FilteredQueue::const_iterator InputEventIterator, bool InManipulationTEST);

MatchResult MatchSpace(const InputEventQueue::FilteredQueue & Queue, InputEventQueue::FilteredQueue::const_iterator InputEventIterator);

MatchResult MatchTap2(const InputEventQueue::FilteredQueue & Queue, InputEventQueue::FilteredQueue::const_iterator InputEventIterator);
MatchResult MatchDoubleTap2(const InputEventQueue::FilteredQueue & Queue, InputEventQueue::FilteredQueue::const_iterator InputEventIterator);

class GestureRecognizer
	: public InputHandler,
	  public MutuallyConnectable<GestureRecognizer, Pointer>
{
public:
	GestureRecognizer(GestureHandler & Owner);
	~GestureRecognizer();

	MatchResult MatchEventQueue(InputEventQueue::FilteredQueue & UnreservedEvents);
	bool ProcessEventHandledTEST(InputEvent InputEvent);
	void ProcessEvent(InputEvent & InputEvent) override;

	virtual void ProcessCanvasUpdated();

	GestureHandler & GetOwner();

	bool				m_RecognizeTap = false;
	bool				m_RecognizeDoubleTap = false;
	bool				m_RecognizeManipulationTranslate = false;
	bool				m_RecognizeScroll = false;

	Vector2n			m_ManipulationOffset;

	struct ShortcutEntry {
		Input::InputId InputId;
		PointerState::Modifiers Modifiers;
		std::function<void()> Action;

		ShortcutEntry(Input::InputId InputId, PointerState::Modifiers Modifiers, std::function<void()> Action)
			: InputId(InputId), Modifiers(Modifiers), Action(Action)
		{}
	};

	void AddShortcut(ShortcutEntry ShortcutEntry);

private:
	GestureRecognizer(const GestureRecognizer &);
	GestureRecognizer & operator = (const GestureRecognizer &);

	bool ProcessShortcuts(InputEvent InputEvent);

	PointerState		m_LastTapStateTEST;
	PointerState		m_LastTapCompletedStateTEST;

	bool				m_InManipulation = false;

	std::list<ShortcutEntry> m_Shortcuts;

	GestureHandler &			m_Owner;
};

#endif // __GestureRecognizer_H__
