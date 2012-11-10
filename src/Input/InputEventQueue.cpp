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

void InputEventQueue::EraseEventsFromQueue(const FilteredQueue & Events)
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

// Pre: Start must be an iterator of the given FilteredQueue
InputEventQueue::FilteredQueue InputEventQueue::CreateFilteredQueue(const FilteredQueue & In, const FilteredQueue::const_iterator Start)
{
	FilteredQueue Out;

	for (auto InputEventIterator = Start; In.end() != InputEventIterator; ++InputEventIterator)
	{
		Out.push_back(*InputEventIterator);
	}

	return Out;
}

void InputEventQueue::EraseEventsFromFilteredQueue(FilteredQueue & InOut, const FilteredQueue & Events)
{
	for (auto & Event : Events)
	{
		for (auto InIterator = InOut.begin(); InOut.end() != InIterator; )
		{
			if (Event == *InIterator)
			{
				InIterator = InOut.erase(InIterator);
			}
			else
			{
				++InIterator;
			}
		}
	}
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
