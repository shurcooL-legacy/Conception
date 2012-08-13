#pragma once
#ifndef __InputListener_H__
#define __InputListener_H__

class InputListener
{
public:
	InputListener();
	virtual ~InputListener();

	virtual void ProcessEvent(const InputEvent & InputEvent) {}

	virtual void ProcessTimePassed(const double TimePassed) {}

private:
	InputListener(const InputListener &);
	InputListener & operator = (const InputListener &);
};

#endif // __InputListener_H__
