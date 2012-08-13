#include "../Main.h"

GestureRecognizer::GestureRecognizer(GestureHandler & Owner)
	: InputHandler(),
	  m_RecognizeTap(false),
	  m_RecognizeManipulationTranslate(false),
	  m_InManipulation(false),
	  m_Owner(Owner)
{
//printf("GESTURE REC Created()!\n");
}

GestureRecognizer::~GestureRecognizer()
{
//printf("GESTURE REC ~Destroyed()!\n");
}

void GestureRecognizer::ProcessEvent(InputEvent & InputEvent)
{
	if (true == InputEvent.m_Handled)
	{
		return;
	}

	// DECISION
#if 1
	// TEST: Capture the pointer if the pointer is activated (via primary button)
	if (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::POINTER_ACTIVATION)
		&& (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::BUTTON_EVENT)
			&& 0 == InputEvent.m_InputId
			&& true == InputEvent.m_Buttons[0]))
	{
		InputEvent.m_Pointer->ModifyPointerMapping().RequestPointerCapture(this);
	}
#endif

	// Release the pointer capture if the pointer is deactivated
	if (InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::POINTER_DEACTIVATION))
	{
		InputEvent.m_Pointer->ModifyPointerMapping().RequestPointerRelease(this);
	}

	// TODO: Fix bug where dragging the object out of down-event range and then bringing it back still activates the tap (it shouldn't)
	if (m_RecognizeTap)
	{
		if (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::BUTTON_EVENT)
			&& 0 == InputEvent.m_InputId
			&& true == InputEvent.m_Buttons[0])
		{
			m_LastTapStateTEST = InputEvent.m_Pointer->GetPointerState();
		}
		else if (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::BUTTON_EVENT)
				 && 0 == InputEvent.m_InputId
				 && false == InputEvent.m_Buttons[0]
				 //&& std::fabs(InputEvent.m_Timestamp - m_LastTapEventTEST.m_Timestamp) <= 1.0)
				 && (Vector2n(InputEvent.m_Pointer->GetPointerState().GetAxisState(0).GetPosition(), InputEvent.m_Pointer->GetPointerState().GetAxisState(1).GetPosition()) - Vector2n(m_LastTapStateTEST.GetAxisState(0).GetPosition(), m_LastTapStateTEST.GetAxisState(1).GetPosition())).LengthSquared() <= (3 * 3))
		{
			printf("Recognized a tap.\n");
			InputEvent.m_Handled = true;
			m_Owner.ProcessTap();
		}
	}

	//if (m_RecognizeDoubleTap)
	{
	}

	//if (m_RecognizeDrag)
	/*{
		if (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::AXIS_EVENT)
			&& 0 == InputEvent.m_InputId
			&& true == InputEvent.GetPointerState().GetButtonState(0))
		{
			//printf("Recognized a drag by (%d, %d).\n", InputEvent.m_Sliders[0], InputEvent.m_Sliders[1]);

			//double Scaler = GetAttached().size();
			//printf("GetAttached().size() %d\n", GetAttached().size());

			//m_Owner.ProcessDrag(Vector2d(InputEvent.m_Sliders[0] / Scaler, InputEvent.m_Sliders[1] / Scaler));
			m_Owner.ProcessDrag(Vector2d(InputEvent.m_Sliders[0], InputEvent.m_Sliders[1]));		// TODO: Figure out this GetAttached() stuff
		}
	}*/

	//if (m_RecognizeScroll)
	{
		if (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::AXIS_EVENT)
			&& 2 == InputEvent.m_InputId)
		{
			//printf("Recognized a wheel move by %d.\n", InputEvent.m_Sliders[0]);
			m_Owner.ProcessScroll(InputEvent, Vector2n(InputEvent.m_Sliders[0], 0));
		}
	}

	// TODO: Support for manipulation with >1 pointer simultaneously (including translation, as well as rotation/scale)
	if (m_RecognizeManipulationTranslate)
	{
		if (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::BUTTON_EVENT)
			&& 0 == InputEvent.m_InputId
			&& true == InputEvent.m_Buttons[0])
		{
			m_InManipulation = true;
			m_Owner.ProcessManipulationStarted(InputEvent.m_Pointer->GetPointerState());
			// DECISION
			//InputEvent.m_Pointer->ModifyPointerMapping().RequestPointerCapture(this);		// TEST
		}

		if (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::AXIS_EVENT)
			&& 0 == InputEvent.m_InputId
			&& true == InputEvent.m_Pointer->GetPointerState().GetButtonState(0))
		{
			//double Scaler = GetAttached().size();

			m_Owner.ProcessManipulationUpdated(InputEvent.m_Pointer->GetPointerState());
			// DECISION
			//InputEvent.m_Pointer->ModifyPointerMapping().RequestPointerCapture(this);		// TEST
		}

		if (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::BUTTON_EVENT)
			&& 0 == InputEvent.m_InputId
			&& false == InputEvent.m_Buttons[0])
		{
			m_InManipulation = false;
			m_Owner.ProcessManipulationCompleted(InputEvent.m_Pointer->GetPointerState());
			// DECISION
			//InputEvent.m_Pointer->ModifyPointerMapping().RequestPointerRelease(this);		// TEST
		}
	}

	//if (...)
	{
		if (InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::CHARACTER_EVENT))
		{
			m_Owner.ProcessCharacter(InputEvent, InputEvent.m_InputId);
		}
	}

	// TODO: There might be duplication here this way, I think I should eliminate it (by providing a complete alternative gesture-level api for all events
	// Low-level event passthrough
	m_Owner.ProcessEvent(InputEvent);
}

void GestureRecognizer::ProcessCanvasUpdated()
{
	if (   m_InManipulation
		&& !GetConnected().empty())
	{
		// HACK: Using the first pointer is a hack
		// TODO: I should pass all connected pointers as a 'frame', they're all relevant
		m_Owner.ProcessManipulationUpdated((*GetConnected().begin())->GetPointerState());
	}
}

GestureHandler & GestureRecognizer::GetOwner()
{
	return m_Owner;
}
