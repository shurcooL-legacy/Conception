#pragma once
#ifndef __Behavior_H__
#define __Behavior_H__

class Behavior
	: public GestureHandler
{
protected:
	Behavior(Widget & Widget);

	Widget & m_Widget;

public:
	virtual ~Behavior();

	virtual void SetupGestureRecognizer() = 0;
	virtual void UnsetupGestureRecognizer() = 0;

private:
	Behavior(const Behavior &) = delete;
	Behavior & operator = (const Behavior &) = delete;
};

#endif // __Behavior_H__
