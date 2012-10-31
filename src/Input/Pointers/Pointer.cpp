#include "../../Main.h"

Pointer::Pointer(InputHandler & MyInputHandler)
	: InputHandler(),
	  m_IsActive(false),
	  m_PointerState(),
	  m_InputHandler(MyInputHandler),
	  m_PointerMapping(*this)
{
	{
		InputEvent InputEvent;
		InputEvent.m_EventTypes.insert(InputEvent::EventType::POINTER_CREATION);
		InputEvent.m_Pointer = this;

		m_InputHandler.ProcessEvent(InputEvent);
	}
}

Pointer::~Pointer()
{
	{
		InputEvent InputEvent;
		InputEvent.m_EventTypes.insert(InputEvent::EventType::POINTER_DELETION);
		InputEvent.m_Pointer = this;

		m_InputHandler.ProcessEvent(InputEvent);
	}
}

void Pointer::ProcessActivation(InputEvent & InputEvent)
{
	m_IsActive = true;

	InputEvent.m_EventTypes.insert(InputEvent::EventType::POINTER_ACTIVATION);
}

void Pointer::ProcessDeactivation(InputEvent & InputEvent)
{
	m_IsActive = false;

	InputEvent.m_EventTypes.insert(InputEvent::EventType::POINTER_DEACTIVATION);
}

bool Pointer::IsActive()
{
	return m_IsActive;
}

void Pointer::ProcessEvent(InputEvent & InputEvent)
{
	//std::cout << InputEvent.ToString();

	for (auto EventType : InputEvent.m_EventTypes)
	{
		switch (EventType)
		{
		case InputEvent::EventType::BUTTON_EVENT:
			{
				auto ButtonId = InputEvent.m_InputId;
				bool Pressed = InputEvent.m_Buttons[0];		// TODO: Check if there are >1 buttons

				// If no buttons were pressed thus far, then the pointer is getting activated
				if (Pressed && !/*InputEvent.m_Pointer->*/GetPointerState().IsAnyButtonsPressed())
				{
					Pointer::ProcessActivation(InputEvent);
				}

				/*InputEvent.m_Pointer->*/ModifyPointerState().UpdateButtonState(ButtonId) = Pressed;

				// If no buttons are pressed anymore, then the pointer is getting deactivated
				if (!Pressed && !/*InputEvent.m_Pointer->*/GetPointerState().IsAnyButtonsPressed())
				{
					Pointer::ProcessDeactivation(InputEvent);
				}
			}
			break;
		case InputEvent::EventType::AXIS_EVENT:
			{
				for (Input::InputId AxisId = 0; AxisId < InputEvent.m_Axes.size(); ++AxisId)
				{
					int AxisPosition = InputEvent.m_Axes[AxisId].GetPosition();
					int PreviousAxisPosition = /*InputEvent.m_Pointer->*/GetPointerState().GetAxisState(InputEvent.m_InputId + AxisId, InputEvent.m_Axes[AxisId]).GetPosition();
					int SliderValue = AxisPosition - PreviousAxisPosition;

					/*InputEvent.m_Pointer->*/ModifyPointerState().UpdateAxisState(InputEvent.m_InputId + AxisId) = InputEvent.m_Axes[AxisId];

					InputEvent.m_Sliders.push_back(SliderValue);
				}
			}
			break;
		default:
			break;
		}
	}

	// TODO: Figure out if this does anything... it looks like a NOOP, so I'm disabling it
	//m_PointerState = InputEvent.m_Pointer->GetPointerState();

	m_InputHandler.ProcessEvent(InputEvent);
}

void Pointer::ProcessTimePassed(const double TimePassed)
{
	// TODO: Make this event only occur when needed rather than always
	// DECISION
	{
		InputEvent InputEvent;
		InputEvent.m_EventTypes.insert(InputEvent::EventType::CANVAS_MOVED_TEST);
		InputEvent.m_Pointer = this;

		InputEvent.m_Pointer->ProcessEvent(InputEvent);
	}
}

const PointerMapping & Pointer::GetPointerMapping()
{
	return m_PointerMapping;
}

PointerMapping & Pointer::ModifyPointerMapping()
{
	return m_PointerMapping;
}

const PointerState & Pointer::GetPointerState()
{
	return m_PointerState;
}

PointerState & Pointer::ModifyPointerState()
{
	return m_PointerState;
}
