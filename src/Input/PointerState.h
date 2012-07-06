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

	auto GetTimestamp() const -> decltype(m_Timestamp);
	auto GetButtonState(Input::InputId ButtonId, bool Default = false) const -> bool;
	auto GetAxisState(Input::InputId AxisId, Input::AxisState Default = Input::AxisState()) const -> Input::AxisState;

	void UpdateTimestamp();
	auto UpdateButtonState(Input::InputId ButtonId) -> decltype((m_ButtonStates[ButtonId]));
	auto UpdateAxisState(Input::InputId AxisId) -> decltype((m_AxisStates[AxisId]));

	bool AnyButtonsPressed() const;
};

#endif // __PointerState_H__
