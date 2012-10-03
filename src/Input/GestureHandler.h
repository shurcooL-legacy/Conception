#pragma once
#ifndef __GestureHandler_H__
#define __GestureHandler_H__

class GestureHandler
	: public InputHandler
{
public:
	GestureHandler();
	~GestureHandler();

	virtual void ProcessTap(InputEvent & InputEvent, Vector2n Position) {}
	virtual void ProcessDoubleTap(InputEvent & InputEvent, Vector2n Position) {}
	//virtual void ProcessDrag(Vector2d DragAmount) {}		// TODO: Sort out the floating vs. integer discrepancy
	virtual void ProcessScroll(InputEvent & InputEvent, Vector2n ScrollAmount) {}

	virtual void ProcessManipulationStarted(const PointerState & PointerState) {}
	virtual void ProcessManipulationUpdated(const PointerState & PointerState) {}
	virtual void ProcessManipulationCompleted(const PointerState & PointerState) {}

	virtual void ProcessCharacter(InputEvent & InputEvent, const uint32 Character) {}

	//virtual void ProcessButton(Pointer * Pointer, Input::InputId ButtonId, bool Pressed) {}
	//virtual void ProcessSlider(Pointer * Pointer, Input::InputId SliderId, double MovedAmount) {}

	// THINK: Does it make sense to derive from InputListener and have `ProcesEvent()` low-level input process function here? Or should I have high-level events only, and make button presses available as trivial gestures?

private:
	GestureHandler(const GestureHandler &);
	GestureHandler & operator = (const GestureHandler &);
};

#endif // __GestureHandler_H__
