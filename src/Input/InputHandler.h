#pragma once
#ifndef __InputHandler_H__
#define __InputHandler_H__

class InputHandler
{
public:
	InputHandler();
	virtual ~InputHandler();

	virtual void ProcessEvent(InputEvent & InputEvent) {}

	virtual void ProcessTimePassed(const double TimePassed) {}

private:
//	InputHandler(const InputHandler &) = delete;
//	InputHandler & operator = (const InputHandler &) = delete;
};

#endif // __InputHandler_H__
