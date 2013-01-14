#pragma once
#ifndef __InputEventQueue_H__
#define __InputEventQueue_H__

class InputEventQueue
	: public InputHandler
{
public:
	typedef std::list<InputEvent> Queue;
	typedef std::list<Queue::iterator> FilteredQueue;		// TODO: Turn this back to const_iterator once Linux C++11 support catches up

	InputEventQueue();
	~InputEventQueue();

	void ProcessEvent(InputEvent & InputEvent) override;

	const Queue & GetQueue() const { return m_Queue; }
	Queue & ModifyQueue() { return m_Queue; }

	void EraseEventsFromQueue(const FilteredQueue & Events);

	FilteredQueue CreateFilteredQueue();		// TODO: Turn this back to const once Linux C++11 support catches up

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
	friend class DebugOverlayWidget;
};

#endif // __InputEventQueue_H__
