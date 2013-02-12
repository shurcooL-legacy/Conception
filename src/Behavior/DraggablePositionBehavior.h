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
	void UnsetupGestureRecognizer() override;

	void ProcessManipulationBegin(const InputEvent & InputEvent) override;
	void ProcessManipulationUpdate(const InputEvent & InputEvent) override;
	void ProcessManipulationEnd(const InputEvent & InputEvent) override;

	void ProcessTimePassed(const double TimePassed) override;

private:
	DraggablePositionBehavior(const DraggablePositionBehavior &) = delete;
	DraggablePositionBehavior & operator = (const DraggablePositionBehavior &) = delete;
};

#endif // __DraggablePositionBehavior_H__
