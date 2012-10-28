#pragma once
#ifndef __InputEventQueue_H__
#define __InputEventQueue_H__

class InputEventQueue
	: public InputHandler
{
public:
	InputEventQueue();
	~InputEventQueue();

	void ProcessEvent(InputEvent & InputEvent) override;

private:
	InputEventQueue(const InputEventQueue &) = delete;
	InputEventQueue & operator = (const InputEventQueue &) = delete;

	void EnqueueEvent(InputEvent & InputEvent);

	// TODO: A circular buffer might be more appropriate here
	std::forward_list<InputEvent>		m_Queue;

	friend class App;		// DEBUG: For info printing
};

#endif // __InputEventQueue_H__
