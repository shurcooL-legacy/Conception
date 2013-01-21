#pragma once
#ifndef __ToggleWidget_H__
#define __ToggleWidget_H__

class ToggleWidget
	: public ButtonWidget
{
public:
	ToggleWidget(Vector2n Position, Vector2n Dimensions, std::function<void(bool)> OnChange, bool State);
	ToggleWidget(Vector2n Position, std::function<void(bool)> OnChange, bool State)
		: ToggleWidget(Position, Vector2n(40, 16), OnChange, State) {}
	virtual ~ToggleWidget();

	void Render() override;

	// TODO: Fix problem where upon initialize, OnChange is called before this widget has been added, so 2nd last widget is not hidden
	void UpdateHACK();

private:
	bool m_State;

	std::function<void(bool)> m_OnChange;
};

#endif // __ToggleWidget_H__
