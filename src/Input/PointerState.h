#pragma once
#ifndef __PointerState_H__
#define __PointerState_H__

class PointerState
{
private:
	double								m_Timestamp;

	std::vector<bool>					m_ButtonStates;		// True means pressed
	std::vector<Input::AxisState>		m_AxisStates;

public:
	PointerState();
	~PointerState();

	decltype(m_Timestamp) GetTimestamp() const;
	auto GetButtonState(Input::InputId ButtonId, bool Default = false) const -> bool;
	auto GetAxisState(Input::InputId AxisId, Input::AxisState Default = Input::AxisState()) const -> Input::AxisState;

	auto UpdateButtonState(Input::InputId ButtonId) -> decltype((m_ButtonStates[ButtonId]));
	auto UpdateAxisState(Input::InputId AxisId) -> decltype((m_AxisStates[AxisId]));

	bool IsAnyButtonsPressed() const;
	bool IsActive() const;

	enum class Modifiers : uint8 {
		None = 0,
		Shift,		// TODO: Make sure that Shift belongs here...
		Ctrl,
		Alt,
		Super
	};
	Modifiers GetModifiers() const;

	void InvalidateTEST();
	
private:
	void UpdateTimestamp();
};

#endif // __PointerState_H__
