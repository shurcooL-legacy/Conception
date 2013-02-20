#include "../Main.h"

PointerState::PointerState()
	: m_Timestamp(glfwGetTime()),
	  m_ButtonStates(),
	  m_AxisStates()
{
}

PointerState::~PointerState()
{
}

decltype(PointerState::m_Timestamp) PointerState::GetTimestamp() const
{
	return m_Timestamp;
}

auto PointerState::GetButtonState(Input::InputId ButtonId, bool Default) const -> bool
{
	if (ButtonId < m_ButtonStates.size())
		return m_ButtonStates[ButtonId];
	else
		return Default;
}

auto PointerState::GetAxisState(Input::InputId AxisId, Input::AxisState Default) const -> Input::AxisState
{
	if (AxisId < m_AxisStates.size())
		return m_AxisStates[AxisId];
	else
		return Default;
}

void PointerState::UpdateTimestamp()
{
	m_Timestamp = glfwGetTime();
}

auto PointerState::UpdateButtonState(Input::InputId ButtonId) -> decltype((m_ButtonStates[ButtonId]))
{
	UpdateTimestamp();

	// Expand the buttons array to fit this button, if neccessary
	while (m_ButtonStates.size() <= ButtonId)
		m_ButtonStates.push_back(false);

	return m_ButtonStates[ButtonId];
}

auto PointerState::UpdateAxisState(Input::InputId AxisId) -> decltype((m_AxisStates[AxisId]))
{
	UpdateTimestamp();

	// Expand the array to fit this, if neccessary
	while (m_AxisStates.size() <= AxisId)
		m_AxisStates.push_back(Input::AxisState());

	return m_AxisStates[AxisId];
}

// Returns true if any of the buttons are pressed, otherwise false
bool PointerState::IsAnyButtonsPressed() const
{
	for (const auto & ButtonState : m_ButtonStates)
	{
		if (ButtonState)
			return true;
	}
	
	return false;
}

// A pointer is defined to be active if any of its buttons are pressed down
bool PointerState::IsActive() const
{
	return IsAnyButtonsPressed();
}

PointerState::Modifiers PointerState::GetModifiers() const
{
	bool Super = GetButtonState(GLFW_KEY_LSUPER) || GetButtonState(GLFW_KEY_RSUPER);

	// TODO: Unfinished...
	if (Super)
		return Modifiers::Super;
	else
		return Modifiers::None;
}

void PointerState::InvalidateTEST()
{
	m_Timestamp = -1000;
}
