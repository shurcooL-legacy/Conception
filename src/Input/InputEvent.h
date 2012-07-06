#pragma once
#ifndef __InputEvent_H__
#define __InputEvent_H__

class InputEvent
{
public:
	InputEvent();
	~InputEvent();

	enum class EventType : uint8 { BUTTON_EVENT, CHARACTER_EVENT, AXIS_EVENT, POINTER_CREATION, POINTER_DELETION, POINTER_ACTIVATION, POINTER_DEACTIVATION, CANVAS_MOVED_TEST, PARENT_SIZE };

	//uint8					m_EventType;
	//EventType				m_EventType;
	std::set<EventType>		m_EventTypes;
	uint8					m_DeviceId;
	Input::InputId			m_InputId;

	std::vector<bool>					m_Buttons;
	std::vector<sint32>					m_Sliders;
	std::vector<Input::AxisState>		m_Axes;

	Pointer *				m_Pointer;

	bool					m_Handled;		// TEST

	std::string ToString();

private:
	InputEvent(const InputEvent &);
	//InputEvent & operator =(const InputEvent &);
};

#endif // __InputEvent_H__
