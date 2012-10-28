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
	m_Queue.push_front(InputEvent);

	//printf("InputEvent %d pushed.\n", *InputEvent.m_EventTypes.begin());
}
