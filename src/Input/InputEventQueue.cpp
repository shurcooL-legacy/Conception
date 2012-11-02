#include "../Main.h"

InputEventQueue::InputEventQueue()
	: InputHandler(),
	  m_Queue()
{
}

InputEventQueue::~InputEventQueue()
{
}

void InputEventQueue::ProcessEvent(InputEvent & InputEvent)
{
	EnqueueEvent(InputEvent);
}

void InputEventQueue::EnqueueEvent(InputEvent & InputEvent)
{
	// Latest events go to queue back
	m_Queue.push_back(InputEvent);

	//printf("InputEvent %d pushed.\n", *InputEvent.m_EventTypes.begin());
}

void InputEventQueue::EraseEventsFromQueue(FilteredQueue & Events)
{
	for (auto & Event : Events)
	{
		m_Queue.erase(Event);
	}
}

InputEventQueue::FilteredQueue InputEventQueue::CreateFilteredQueue() const
{
	FilteredQueue Out;

	for (auto InputEventIterator = m_Queue.begin(); m_Queue.end() != InputEventIterator; ++InputEventIterator)
	{
		Out.push_back(InputEventIterator);
	}

	return Out;
}

InputEventQueue::FilteredQueue InputEventQueue::CreateFilteredQueue(const Queue & Queue, Queue::const_iterator Start)
{
	FilteredQueue Out;

	for (auto InputEventIterator = Start; Queue.end() != InputEventIterator; ++InputEventIterator)
	{
		Out.push_back(InputEventIterator);
	}

	return Out;
}

InputEventQueue::FilteredQueue InputEventQueue::FilterByPointer(const FilteredQueue & In, const Pointer * Pointer)
{
	FilteredQueue Out;

	for (auto & InputEventIterator : In)
	{
		if (Pointer == InputEventIterator->m_Pointer)
			Out.push_back(InputEventIterator);
	}

	return Out;
}
