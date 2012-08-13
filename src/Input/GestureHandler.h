#pragma once
#ifndef __GestureListener_H__
#define __GestureListener_H__

class GestureListener
	: public InputListener
{
public:
	GestureListener();
	~GestureListener();

	virtual void ProcessTap() {}
	//virtual void ProcessDrag(Vector2d DragAmount) {}		// TODO: Sort out the floating vs. integer discrepancy
	virtual void ProcessScroll(InputEvent & InputEvent, Vector2n ScrollAmount) {}

	virtual void ProcessManipulationStarted(const PointerState & PointerState) {}
	virtual void ProcessManipulationUpdated(const PointerState & PointerState) {}
	virtual void ProcessManipulationCompleted(const PointerState & PointerState) {}

	virtual void ProcessCharacter(const uint32 Character) {}

	//virtual void ProcessButton(Pointer * Pointer, Input::InputId ButtonId, bool Pressed) {}
	//virtual void ProcessSlider(Pointer * Pointer, Input::InputId SliderId, double MovedAmount) {}

	// THINK: Does it make sense to derive from InputListener and have this low-level input process function here? Or should I have high-level events only, and make button presses available as trivial gestures?
	virtual void ProcessEvent(InputEvent & InputEvent) override {}

	virtual void ProcessTimePassed(const double TimePassed) override {}

private:
	GestureListener(const GestureListener &);
	GestureListener & operator = (const GestureListener &);
};

#endif // __GestureListener_H__
