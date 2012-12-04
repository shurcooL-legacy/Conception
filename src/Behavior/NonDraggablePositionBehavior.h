#pragma once
#ifndef __NonDraggablePositionBehavior_H__
#define __NonDraggablePositionBehavior_H__

class NonDraggablePositionBehavior
	: public Behavior
{
public:
	NonDraggablePositionBehavior(Widget & Widget);
	virtual ~NonDraggablePositionBehavior();

	void SetupGestureRecognizer() override;

private:
	NonDraggablePositionBehavior(const NonDraggablePositionBehavior &) = delete;
	NonDraggablePositionBehavior & operator = (const NonDraggablePositionBehavior &) = delete;
};

#endif // __NonDraggablePositionBehavior_H__
