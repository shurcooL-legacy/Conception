#pragma once
#ifndef __InputEvent_H__
#define __InputEvent_H__

class InputEvent
{
public:
	enum class EventType : uint8 {
		BUTTON_EVENT,
		CHARACTER_EVENT,
		AXIS_EVENT,
		POINTER_CREATION, POINTER_DELETION,
		POINTER_ACTIVATION, POINTER_DEACTIVATION,
		CANVAS_MOVED_TEST,
		PARENT_SIZE
	};

	InputEvent();
	~InputEvent();

	//EventType				m_EventType;
	std::set<EventType>		m_EventTypes;
	uint8					m_DeviceId;
	Input::InputId			m_InputId;

	std::vector<bool>					m_Buttons;
	std::vector<sint32>					m_Sliders;
	std::vector<Input::AxisState>		m_Axes;

	Pointer *				m_Pointer;

	PointerState			m_PreEventState;		// State of pointer before this event, needed for queue processing
	double					m_Timestamp;			// Timestamp of event

	bool					m_Handled;		// TEST

	decltype(m_Timestamp) GetTimestamp() const { return m_Timestamp; }

	bool HasType(EventType EventType) const;

	std::string ToString() const;

private:
	//InputEvent(const InputEvent &) = delete;
	//InputEvent & operator = (const InputEvent &) = delete;
};

#endif // __InputEvent_H__
