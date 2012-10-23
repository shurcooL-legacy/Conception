#pragma once
#ifndef __DraggablePositionBehavior_H__
#define __DraggablePositionBehavior_H__

class DraggablePositionBehavior
	: public Behavior
{
public:
	DraggablePositionBehavior(Widget & Widget);
	virtual ~DraggablePositionBehavior();

	void SetupGestureRecognizer() override;

	void ProcessManipulationStarted(const PointerState & PointerState) override;
	void ProcessManipulationUpdated(const PointerState & PointerState) override;
	void ProcessManipulationCompleted(const PointerState & PointerState) override;

private:
	DraggablePositionBehavior(const DraggablePositionBehavior &) = delete;
	DraggablePositionBehavior & operator = (const DraggablePositionBehavior &) = delete;
};

#endif // __DraggablePositionBehavior_H__
