#pragma once
#ifndef __InputResponder_H__
#define __InputResponder_H__

class InputResponder
{
public:
	InputResponder();
	virtual ~InputResponder();

	virtual bool ProcessEvent(InputEvent & InputEvent) { return false; }
	
	virtual void ProcessTimePassed(const double TimePassed) {}

private:
	InputResponder(const InputResponder &) = delete;
	InputResponder & operator = (const InputResponder &) = delete;
};

#endif // __InputResponder_H__
