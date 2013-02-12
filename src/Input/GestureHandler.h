#pragma once
#ifndef __GestureHandler_H__
#define __GestureHandler_H__

enum class GestureType : uint8 {
	SingleTap,
	DoubleTap
};

class GestureHandler
	: public InputHandler
{
public:
	GestureHandler();
	~GestureHandler();

	virtual void ProcessTap(const InputEvent & InputEvent, Vector2n Position) {}
	virtual void ProcessDoubleTap(const InputEvent & InputEvent, Vector2n Position) {}
	virtual void ProcessScroll(InputEvent & InputEvent, Vector2n ScrollAmount) {}

	virtual void ProcessManipulationBegin(const InputEvent & InputEvent) {}
	virtual void ProcessManipulationUpdate(const InputEvent & InputEvent) {}
	virtual void ProcessManipulationEnd(const InputEvent & InputEvent) {}

	virtual void ProcessCharacter(InputEvent & InputEvent, const uint32 Character) {}

	// THINK: Does it make sense to derive from InputHandler and have `ProcesEvent()` low-level input process function here? Or should I have high-level events only, and make button presses available as trivial gestures?

private:
	GestureHandler(const GestureHandler &);
	GestureHandler & operator = (const GestureHandler &);
};

#endif // __GestureHandler_H__
