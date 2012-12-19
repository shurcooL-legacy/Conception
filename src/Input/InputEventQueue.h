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

	void EraseEventsFromQueue(const FilteredQueue & Events);

	FilteredQueue CreateFilteredQueue() const;

	static FilteredQueue CreateFilteredQueue(const FilteredQueue & In, const FilteredQueue::const_iterator Start);
	static void EraseEventsFromFilteredQueue(FilteredQueue & InOut, const FilteredQueue & Events);
	static FilteredQueue FilterByPointer(const FilteredQueue & In, const Pointer * Pointer);

private:
	InputEventQueue(const InputEventQueue &) = delete;
	InputEventQueue & operator = (const InputEventQueue &) = delete;

	void EnqueueEvent(InputEvent & InputEvent);

	// TODO: A circular buffer might be more appropriate here
	Queue		m_Queue;

	// DEBUG: For info printing
	friend class App;
	friend class MultitouchTestApp;
	friend class ConceptionApp;
};

#endif // __InputEventQueue_H__
