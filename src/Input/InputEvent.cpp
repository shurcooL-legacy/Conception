#include "../Main.h"

InputEvent::InputEvent()
	: m_EventTypes(),
	  m_DeviceId(-1),
	  m_InputId(-1),
	  m_Buttons(),
	  m_Sliders(),
	  m_Axes(),
	  m_Pointer(nullptr),
	  m_PostEventState(),
	  m_Timestamp(glfwGetTime()),
	  m_Handled(false)
{
}

InputEvent::~InputEvent()
{
}

bool InputEvent::HasType(EventType EventType) const
{
	return (m_EventTypes.end() != m_EventTypes.find(EventType));
}

std::string InputEvent::ToString() const
{
	std::stringstream ss;

	ss << "IE.{";

	for (auto & EventType : m_EventTypes)
		ss << static_cast<sint32>(EventType) << "/";
	//ss << m_EventTypes.size() << ", ";
	ss << ", ";
	ss << static_cast<sint32>(m_DeviceId) << ", ";
	ss << m_InputId << ", ";

	ss << m_Buttons.size() << ", ";
	ss << m_Sliders.size() << ", ";
	ss << m_Axes.size() << ", ";

	ss << m_PreEventState.GetTimestamp() << ", ";
	ss << m_PostEventState.GetTimestamp() << ", ";

	ss << m_Pointer << ", ";

	ss << m_Handled << "}";

	return ss.str();
}
