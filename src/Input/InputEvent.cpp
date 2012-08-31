#include "../Main.h"

InputEvent::InputEvent()
	: m_Pointer(nullptr),
	  m_Handled(false)
{
}

InputEvent::~InputEvent()
{
}

std::string InputEvent::ToString()
{
	std::stringstream ss;

	ss << "InputEvent: ";

	ss << m_EventTypes.size() << ", ";
	ss << static_cast<sint32>(m_DeviceId) << ", ";
	ss << m_InputId << ", ";

	ss << m_Buttons.size() << ", ";
	ss << m_Sliders.size() << ", ";
	ss << m_Axes.size() << ", ";

	ss << m_Pointer << ", ";

	ss << m_Handled << std::endl;

	return ss.str();
}
