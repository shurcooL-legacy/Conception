#pragma once
#ifndef __InputEventQueue_H__
#define __InputEventQueue_H__

class InputEventQueue
	: public InputHandler
{
public:
	typedef std::list<InputEvent> Queue;
	typedef std::list<Queue::const_iterator> FilteredQueue;

	InputEventQueue();
	~InputEventQueue();

	void ProcessEvent(InputEvent & InputEvent) override;

	const Queue & GetQueue() const { return m_Queue; }
	Queue & ModifyQueue() { return m_Queue; }

	FilteredQueue CreateFilteredQueue() const;

	static FilteredQueue FilterByPointer(const FilteredQueue & In, const Pointer * Pointer);

private:
	InputEventQueue(const InputEventQueue &) = delete;
	InputEventQueue & operator = (const InputEventQueue &) = delete;

	void EnqueueEvent(InputEvent & InputEvent);

	// TODO: A circular buffer might be more appropriate here
	Queue		m_Queue;

	friend class App;		// DEBUG: For info printing
};

#endif // __InputEventQueue_H__
