#include "Main.h"

App::App(InputManager & InputManager)
	: InputHandler(),
	  m_Widgets(),
	  m_InputManager(InputManager)
{
	ModifyInputManager().SetInputHandler(this);
}

App::~App()
{
	printf("~App()\n");

	ModifyInputManager().ClearInputHandler();
}

void App::Layout()
{
	// Invoke a PARENT_SIZE event
	{
		InputEvent InputEvent;
		InputEvent.m_EventTypes.insert(InputEvent::EventType::PARENT_SIZE);		// TODO: Maybe put the event details inside?

		ProcessEvent(InputEvent);
	}
}

const InputManager & App::GetInputManager()
{
	return m_InputManager;
}

InputManager & App::ModifyInputManager()
{
	return m_InputManager;
}

void App::Render()
{
	for (auto & Widget : m_Widgets)
	{
		Widget->Render();
	}
}

template <Pointer::VirtualCategory PointerVirtualCategory, Input::InputId ButtonId, bool ButtonState>
bool IsPointerButtonEvent(const InputEvent & InputEvent)
{
	return (   nullptr != InputEvent.m_Pointer
			&& PointerVirtualCategory == InputEvent.m_Pointer->GetVirtualCategory()
			&& InputEvent.HasType(InputEvent::EventType::BUTTON_EVENT)
			&& ButtonId == InputEvent.m_InputId
			&& ButtonState == InputEvent.m_Buttons[0]);
			//&& !ButtonState == InputEvent.m_PreEventState.GetButtonState(ButtonId));
}

template <Input::InputId InputId>
bool IsPointerPointingMoveEvent(const InputEvent & InputEvent)
{
	return (   nullptr != InputEvent.m_Pointer
			&& Pointer::VirtualCategory::POINTING == InputEvent.m_Pointer->GetVirtualCategory()
			&& (   InputEvent.HasType(InputEvent::EventType::AXIS_EVENT)
				|| InputEvent.HasType(InputEvent::EventType::CANVAS_MOVED_TEST))
			&& InputId == InputEvent.m_InputId);
}

const sint32 TapRadius = 3;
const double TapTime = 10.200;
const double DoubleTapTime = 2 * TapTime;

uint8 MatchDoubleTap(const InputEventQueue::Queue & Queue, InputEventQueue::Queue::const_iterator & InputEventIterator)
{
	if (Queue.end() == InputEventIterator)
		return 1;

	if (IsPointerButtonEvent<Pointer::VirtualCategory::POINTING, 0, true>(*InputEventIterator))
	{
		Vector2n DownPosition(InputEventIterator->m_PreEventState.GetAxisState(0).GetPosition(), InputEventIterator->m_PreEventState.GetAxisState(1).GetPosition());
		auto DownTime = InputEventIterator->GetTimestamp();

		uint8 NumberOfTaps = 0;

		auto InputEventIterator2 = InputEventIterator;
		++InputEventIterator2;
		for (; Queue.end() != InputEventIterator2; ++InputEventIterator2)
		{
			if (IsPointerButtonEvent<Pointer::VirtualCategory::POINTING, 0, false>(*InputEventIterator2))
			{
				Vector2n UpPosition(InputEventIterator2->m_PreEventState.GetAxisState(0).GetPosition(), InputEventIterator2->m_PreEventState.GetAxisState(1).GetPosition());
				auto UpTime = InputEventIterator2->GetTimestamp();

				if (   (UpPosition - DownPosition).LengthSquared() <= (TapRadius * TapRadius)
					&& (UpTime - DownTime) <= DoubleTapTime)
				{
					if (1 == NumberOfTaps)
					{
						++InputEventIterator2;
						InputEventIterator = InputEventIterator2;
						return 2;
					}
					else
						++NumberOfTaps;
				}
				else
				{
					return 0;
				}
			}
			else if (IsPointerPointingMoveEvent<0>(*InputEventIterator2))
			{
				Vector2n MovePosition(InputEventIterator2->m_PreEventState.GetAxisState(0).GetPosition(), InputEventIterator2->m_PreEventState.GetAxisState(1).GetPosition());
				auto NewTime = InputEventIterator2->GetTimestamp();

				if (   (MovePosition - DownPosition).LengthSquared() <= (TapRadius * TapRadius)
					&& (NewTime - DownTime) <= TapTime)
				{}
				else
				{
					return 0;
				}
			}
			else if (IsPointerButtonEvent<Pointer::VirtualCategory::POINTING, 0, true>(*InputEventIterator2))
			{
				Vector2n MovePosition(InputEventIterator2->m_PreEventState.GetAxisState(0).GetPosition(), InputEventIterator2->m_PreEventState.GetAxisState(1).GetPosition());
				auto NewTime = InputEventIterator2->GetTimestamp();

				if (   (MovePosition - DownPosition).LengthSquared() <= (TapRadius * TapRadius)
					&& (NewTime - DownTime) <= TapTime)
				{}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}

		// Only if there's still a chance a new event can come in time to make a match
		if ((glfwGetTime() - DownTime) <= DoubleTapTime)
		{
			return 1;
		}
	}

	return 0;
}

uint8 MatchTap(const InputEventQueue::Queue & Queue, InputEventQueue::Queue::const_iterator & InputEventIterator)
{
	if (Queue.end() == InputEventIterator)
		return 1;

	if (IsPointerButtonEvent<Pointer::VirtualCategory::POINTING, 0, true>(*InputEventIterator))
	{
		Vector2n DownPosition(InputEventIterator->m_PreEventState.GetAxisState(0).GetPosition(), InputEventIterator->m_PreEventState.GetAxisState(1).GetPosition());
		auto DownTime = InputEventIterator->GetTimestamp();

		auto InputEventIterator2 = InputEventIterator;
		++InputEventIterator2;
		for (; Queue.end() != InputEventIterator2; ++InputEventIterator2)
		{
			if (IsPointerButtonEvent<Pointer::VirtualCategory::POINTING, 0, false>(*InputEventIterator2))
			{
				Vector2n UpPosition(InputEventIterator2->m_PreEventState.GetAxisState(0).GetPosition(), InputEventIterator2->m_PreEventState.GetAxisState(1).GetPosition());
				auto UpTime = InputEventIterator2->GetTimestamp();

				if (   (UpPosition - DownPosition).LengthSquared() <= (TapRadius * TapRadius)
					&& (UpTime - DownTime) <= TapTime)
				{
					++InputEventIterator2;
					InputEventIterator = InputEventIterator2;
					return 2;
				}
				else
				{
					return 0;
				}
			}
			else if (IsPointerPointingMoveEvent<0>(*InputEventIterator2))
			{
				Vector2n MovePosition(InputEventIterator2->m_PreEventState.GetAxisState(0).GetPosition(), InputEventIterator2->m_PreEventState.GetAxisState(1).GetPosition());
				auto NewTime = InputEventIterator2->GetTimestamp();

				if (   (MovePosition - DownPosition).LengthSquared() <= (TapRadius * TapRadius)
					&& (NewTime - DownTime) <= TapTime)
				{}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}

		// Only if there's still a chance a new event can come in time to make a match
		if ((glfwGetTime() - DownTime) <= TapTime)
		{
			return 1;
		}
	}

	return 0;
}

struct MatchResult
{
	uint8										Status;		// 0 - Impossible match, 1 = Potential match, 2 = Successful match
	InputEventQueue::Queue::const_iterator		End;
	InputEventQueue::FilteredQueue				Events;

	MatchResult(uint8 Status) : Status(Status) {}
	MatchResult(InputEventQueue::Queue::const_iterator End) : Status(2), End(End), Events() {}
	MatchResult(InputEventQueue::Queue::const_iterator End, InputEventQueue::FilteredQueue & Events) : Status(2), End(End), Events(Events) {}
};

MatchResult MatchDown(const InputEventQueue::Queue & Queue, InputEventQueue::Queue::const_iterator InputEventIterator)
{
	if (Queue.end() == InputEventIterator)
		return MatchResult(1);

	if (IsPointerButtonEvent<Pointer::VirtualCategory::POINTING, 0, true>(*InputEventIterator))
	{
		InputEventQueue::FilteredQueue Events;
		Events.push_back(InputEventIterator);
		++InputEventIterator;
		return MatchResult(InputEventIterator, Events);
	}

	return MatchResult(0);
}

MatchResult MatchUp(const InputEventQueue::Queue & Queue, InputEventQueue::Queue::const_iterator InputEventIterator)
{
	if (Queue.end() == InputEventIterator)
		return MatchResult(1);

	if (IsPointerButtonEvent<Pointer::VirtualCategory::POINTING, 0, false>(*InputEventIterator))
	{
		InputEventQueue::FilteredQueue Events;
		Events.push_back(InputEventIterator);
		++InputEventIterator;
		return MatchResult(InputEventIterator, Events);
	}

	return MatchResult(0);
}

MatchResult MatchSpace(const InputEventQueue::Queue & Queue, InputEventQueue::Queue::const_iterator InputEventIterator)
{
	if (Queue.end() == InputEventIterator)
		return MatchResult(1);

	if (IsPointerButtonEvent<Pointer::VirtualCategory::TYPING, GLFW_KEY_SPACE, true>(*InputEventIterator))
	{
		InputEventQueue::FilteredQueue Events;
		Events.push_back(InputEventIterator);
		++InputEventIterator;
		return MatchResult(InputEventIterator, Events);
	}

	return MatchResult(0);
}

MatchResult MatchTap2(const InputEventQueue::Queue & Queue, InputEventQueue::Queue::const_iterator InputEventIterator)
{
	auto DownMatch = MatchDown(Queue, InputEventIterator);
	if (2 == DownMatch.Status)
	{
		Vector2n DownPosition(InputEventIterator->m_PreEventState.GetAxisState(0).GetPosition(), InputEventIterator->m_PreEventState.GetAxisState(1).GetPosition());
		auto DownTime = InputEventIterator->GetTimestamp();
		auto PointingPointer = InputEventIterator->m_Pointer;

		for (auto & InputEventIterator2 : InputEventQueue::FilterByPointer(InputEventQueue::CreateFilteredQueue(Queue, DownMatch.End), PointingPointer))
		//for (auto InputEventIterator2 = DownMatch.End; Queue.end() != InputEventIterator2; ++InputEventIterator2)
		{
			DownMatch.Events.push_back(InputEventIterator2);

			if (IsPointerButtonEvent<Pointer::VirtualCategory::POINTING, 0, false>(*InputEventIterator2))
			{
				Vector2n UpPosition(InputEventIterator2->m_PreEventState.GetAxisState(0).GetPosition(), InputEventIterator2->m_PreEventState.GetAxisState(1).GetPosition());
				auto UpTime = InputEventIterator2->GetTimestamp();

				if (   (UpPosition - DownPosition).LengthSquared() <= (TapRadius * TapRadius)
					&& (UpTime - DownTime) <= TapTime)
				{
					++InputEventIterator2;
					return MatchResult(InputEventIterator2, DownMatch.Events);
				}
				else
				{
					return MatchResult(0);
				}
			}
			else if (IsPointerPointingMoveEvent<0>(*InputEventIterator2))
			{
				Vector2n MovePosition(InputEventIterator2->m_PreEventState.GetAxisState(0).GetPosition(), InputEventIterator2->m_PreEventState.GetAxisState(1).GetPosition());
				auto NewTime = InputEventIterator2->GetTimestamp();

				if (   (MovePosition - DownPosition).LengthSquared() <= (TapRadius * TapRadius)
					&& (NewTime - DownTime) <= TapTime)
				{}
				else
				{
					return MatchResult(0);
				}
			}
			else
			{
				return MatchResult(0);
			}
		}

		// Only if there's still a chance a new event can come in time to make a match
		if ((glfwGetTime() - DownTime) <= TapTime)
		{
			return MatchResult(1);
		}
	}
	else
	{
		return DownMatch;
	}

	return MatchResult(0);
}

/*uint8 MatchDoubleTap2(const InputEventQueue::Queue & Queue, InputEventQueue::Queue::const_iterator & InputEventIterator)
{
	auto FirstTap = MatchTap2(Queue, InputEventIterator);
	if (2 == FirstTap)
	{
		auto SecondTap = MatchTap2(Queue, InputEventIterator);
		if (2 == SecondTap)
		{
			return 2;
		}
		else if (1 == SecondTap)
		{
			return 1;
		}
		else if (0 == SecondTap)
		{
			return 0;
		}
	}
	else if (1 == FirstTap)
	{
		return 1;
	}
	else if (0 == FirstTap)
	{
		return 0;
	}

	return 0;
}*/

void App::ProcessEventQueue(InputEventQueue & InputEventQueue)
{
//return;

	// Pass it through widgets

	{
		//for (auto & InputEvent : InputEventQueue.Queue())
		//for (auto InputEventIterator = InputEventQueue.GetQueue().begin(); InputEventQueue.GetQueue().end() != InputEventIterator; )
		while (!InputEventQueue.GetQueue().empty())
		{
			auto InputEventIterator = InputEventQueue.GetQueue().begin();

			//decltype(InputEventIterator) InputEventIterator2;
			//uint8 Status;

			/*auto InputEventIterator2 = InputEventIterator;
			auto Status = MatchDoubleTap2(InputEventQueue.GetQueue(), InputEventIterator2);
			if (2 == Status)
			{
				std::cout << "Double Tap at " << InputEventIterator->m_Pointer->GetPointerState().GetAxisState(0).GetPosition() << ","
											  << InputEventIterator->m_Pointer->GetPointerState().GetAxisState(1).GetPosition() << std::endl;

				InputEventIterator = InputEventQueue.ModifyQueue().erase(InputEventIterator, InputEventIterator2);
				continue;
			}
			else if (1 == Status)
			{
				break;
			}
			else if (0 == Status)
			{
			}*/

			auto TapMatch = MatchTap2(InputEventQueue.GetQueue(), InputEventIterator);
			if (2 == TapMatch.Status)
			{
				std::cout << "Tap at " << InputEventIterator->m_Pointer->GetPointerState().GetAxisState(0).GetPosition() << ","
									   << InputEventIterator->m_Pointer->GetPointerState().GetAxisState(1).GetPosition() << std::endl;

				//InputEventIterator = InputEventQueue.ModifyQueue().erase(InputEventIterator, TapMatch.End);		// TODO: Erase only relevant elements, not all in range (i.e. can delete other pointer events by accident)
				InputEventQueue.EraseEventsFromQueue(TapMatch.Events);
				continue;
			}
			else if (1 == TapMatch.Status)
			{
				break;
			}
			else if (0 == TapMatch.Status)
			{
			}

			auto DownMatch = MatchDown(InputEventQueue.GetQueue(), InputEventIterator);
			if (2 == DownMatch.Status)
			{
				std::cout << "Mouse Down at " << InputEventIterator->m_Pointer->GetPointerState().GetAxisState(0).GetPosition() << "," \
											  << InputEventIterator->m_Pointer->GetPointerState().GetAxisState(1).GetPosition() << std::endl;

				//InputEventIterator = InputEventQueue.ModifyQueue().erase(InputEventIterator, DownMatch.End);
				InputEventQueue.EraseEventsFromQueue(DownMatch.Events);
				continue;
			}
			else if (1 == DownMatch.Status)
			{
				break;
			}
			else if (0 == DownMatch.Status)
			{
			}

			auto UpMatch = MatchUp(InputEventQueue.GetQueue(), InputEventIterator);
			if (2 == UpMatch.Status)
			{
				std::cout << "Mouse Up at " << InputEventIterator->m_Pointer->GetPointerState().GetAxisState(0).GetPosition() << "," \
											<< InputEventIterator->m_Pointer->GetPointerState().GetAxisState(1).GetPosition() << std::endl;

				//InputEventIterator = InputEventQueue.ModifyQueue().erase(InputEventIterator, UpMatch.End);
				InputEventQueue.EraseEventsFromQueue(UpMatch.Events);
				continue;
			}
			else if (1 == UpMatch.Status)
			{
				break;
			}
			else if (0 == UpMatch.Status)
			{
			}

			auto SpaceMatch = MatchSpace(InputEventQueue.GetQueue(), InputEventIterator);
			if (2 == SpaceMatch.Status)
			{
				std::cout << "Space" << std::endl;

				//InputEventIterator = InputEventQueue.ModifyQueue().erase(InputEventIterator, UpMatch.End);
				InputEventQueue.EraseEventsFromQueue(SpaceMatch.Events);
				continue;
			}
			else if (1 == SpaceMatch.Status)
			{
				break;
			}
			else if (0 == SpaceMatch.Status)
			{
			}

			InputEventIterator = InputEventQueue.ModifyQueue().erase(InputEventIterator);
		}

		/*for (auto & InputEventIterator : InputEventQueue::FilterByPointer(InputEventQueue.CreateFilteredQueue(), g_InputManager->m_MousePointer.get()))
		{
			std::cout << "2nd loop " << InputEventIterator->ToString() << std::endl;

			if (   InputEventIterator->HasType(InputEvent::EventType::BUTTON_EVENT)
				&& 0 == InputEventIterator->m_InputId
				&& true == InputEventIterator->m_Buttons[0])
			{
				std::cout << "Mouse Down at " << InputEventIterator->m_Pointer->GetPointerState().GetAxisState(0).GetPosition() << ","
											  << InputEventIterator->m_Pointer->GetPointerState().GetAxisState(1).GetPosition() << std::endl;
			}

			if (   InputEventIterator->HasType(InputEvent::EventType::BUTTON_EVENT)
				&& 0 == InputEventIterator->m_InputId
				&& false == InputEventIterator->m_Buttons[0])
			{
				std::cout << "Mouse Up at " << InputEventIterator->m_Pointer->GetPointerState().GetAxisState(0).GetPosition() << ","
											<< InputEventIterator->m_Pointer->GetPointerState().GetAxisState(1).GetPosition() << std::endl;
			}
		}*/
	}
}

void App::ProcessEvent(InputEvent & InputEvent)
{
	if (   InputEvent.HasType(InputEvent::EventType::AXIS_EVENT)
		|| InputEvent.HasType(InputEvent::EventType::CANVAS_MOVED_TEST))
	{
		if (Pointer::VirtualCategory::POINTING == InputEvent.m_Pointer->GetVirtualCategory())
		{
			if (nullptr == InputEvent.m_Pointer->GetPointerMapping().GetCapturer())
			{
				//Pointer->GetPointerMapping().RemoveHoverPointer(Pointer);		// TODO: Maybe only remove/add when there's change, rather than starting from scratch each time

				std::list<Widget *> Hits;		// Front of list are top-most widgets

				for (auto & Widget : reverse(m_Widgets))
				{
					Vector2n GlobalPosition(InputEvent.m_Pointer->GetPointerState().GetAxisState(0).GetPosition(), InputEvent.m_Pointer->GetPointerState().GetAxisState(1).GetPosition());

					auto Result = Widget->HitTest(GlobalPosition, &Hits);
#if DECISION_POINTER_MAPPING_CONTAINS_SINGLE_TOPMOST_WIDGET
					if (Result)
						break;		// HACK: I need to do proper query if the HitTest prevents passthrough interest check
#endif
				}

				InputEvent.m_Pointer->ModifyPointerMapping().RemoveAllMappings();
				for (auto & Hit : Hits)
				{
					InputEvent.m_Pointer->ModifyPointerMapping().AddMapping(Hit->ModifyGestureRecognizer());
					//Hit->AddHoverPointer(Pointer);
				}
				InputEvent.m_Pointer->ModifyPointerMapping().DoneAdding();
			}
		}
	}

	if (InputEvent.HasType(InputEvent::EventType::PARENT_SIZE))
	{
		UpdateWindowDimensions(ModifyInputManager().GetWindowDimensions());
	}

	if (nullptr != InputEvent.m_Pointer)
	{
		InputEvent.m_Pointer->ModifyPointerMapping().ProcessEvent(InputEvent);
	}

	// DEBUG, TEST: System key handling
	if (false == InputEvent.m_Handled)
	{
		if (InputEvent.HasType(InputEvent::EventType::BUTTON_EVENT))
		{
			if (Pointer::VirtualCategory::TYPING == InputEvent.m_Pointer->GetVirtualCategory())
			{
				auto ButtonId = InputEvent.m_InputId;
				bool Pressed = InputEvent.m_Buttons[0];		// TODO: Check if there are >1 buttons

				if (Pressed)
				{
					switch (ButtonId)
					{
					case GLFW_KEY_ESC:
						{
							glfwCloseWindow();

							InputEvent.m_Handled = true;
						}
						break;
					// TEST
					case 'M':
						{
							//if (glfwGetKey(GLFW_KEY_LCTRL) || glfwGetKey(GLFW_KEY_RCTRL))
							{
								//g_InputManager->SetMouseCursorVisibility(!g_InputManager->IsMouseCursorVisible());
								/*int xpos, ypos;
								glfwGetMousePos(&xpos, &ypos);
								glfwSetMousePos(xpos+1, ypos);*/
							}

							InputEvent.m_Handled = true;
						}
						break;
					default:
						break;
					}
				}
			}
		}
	}
}

void App::ProcessTimePassed(const double TimePassed)
{
	for (auto & Widget : m_Widgets)
	{
		Widget->ProcessTimePassed(TimePassed);
	}

	// DEBUG: Print debug info
	{
		std::ostringstream out;

		/*out << "Mouse.PntrMppng.m_Entries.size(): " << g_InputManager->m_MousePointer->ModifyPointerMapping().m_Entries.size();
		for (auto & i : g_InputManager->m_MousePointer->ModifyPointerMapping().m_Entries)
		{
			if (dynamic_cast<Canvas *>(&i->GetOwner())) out << "\n Canvas";
			else if (dynamic_cast<MultitouchTestBoxWidget *>(&i->GetOwner())) out << "\n MultitouchTestBoxWidget, color: " << static_cast<uint16>(static_cast<MultitouchTestBoxWidget *>(&i->GetOwner())->m_Color);
			else if (dynamic_cast<TextFieldWidget *>(&i->GetOwner())) out << "\n TextFieldWidget";
			else if (dynamic_cast<ButtonWidget *>(&i->GetOwner())) out << "\n ButtonWidget";
			else if (dynamic_cast<ListWidget<ConceptId> *>(&i->GetOwner())) out << "\n ListWidget<ConceptId>";

			auto LocalPosition = dynamic_cast<Widget *>(&i->GetOwner())->GlobalToLocal(Vector2n(g_InputManager->m_MousePointer->GetPointerState().GetAxisState(0).GetPosition(), g_InputManager->m_MousePointer->GetPointerState().GetAxisState(1).GetPosition()));
			out << " (" << LocalPosition.X() << ", " << LocalPosition.Y() << ")";
		}*/

		out << "InputManager.m_IEQueue.m_Queue" << std::endl;
		for (auto & i : g_InputManager->m_InputEventQueue.m_Queue)
		{
			out << i.ToString() << std::endl;
		}

		OpenGLStream(Vector2n::ZERO) << out.str();
	}
}

// Return true if the app should render the next frame right away, instead of waiting for next input event
bool App::ShouldRedrawRegardless()
{
	// By default, apps always keep redrawing each frame
	return true;
}

std::string App::GetTitle()
{
	return "Conception";
}
