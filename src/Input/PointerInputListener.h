#pragma once
#ifndef __PointerInputListener_H__
#define __PointerInputListener_H__

#if 0
class PointerInputListener
{
public:
	PointerInputListener();
	virtual ~PointerInputListener();

	virtual void ProcessPointerCreation(Pointer * Pointer) {}
	virtual void ProcessPointerDeletion(Pointer * Pointer) {}
	virtual void ProcessPointerActivation(Pointer * Pointer) {}
	virtual void ProcessPointerDeactivation(Pointer * Pointer) {}

	virtual void ProcessButton(Pointer * Pointer, Input::InputId ButtonId, bool Pressed) {}
	virtual void ProcessSlider(Pointer * Pointer, Input::InputId SliderId, double MovedAmount) {}
	virtual void ProcessAxis(Pointer * Pointer, Input::InputId AxisId, Input::AxisState AxisState) {}
	virtual void Process2Axes(Pointer * Pointer, Input::InputId FirstAxisId, Input::AxisState AxisState[2]) {}

	virtual void ProcessCharacter(Pointer * Pointer, int Character) {}
	virtual void ProcessPosition(Pointer * Pointer, Vector2n Position) {}

	virtual void ProcessTimePassed(const double TimePassed) {}

private:
	PointerInputListener(const PointerInputListener &);
	PointerInputListener & operator = (const PointerInputListener &);
};
#endif

#endif // __PointerInputListener_H__
