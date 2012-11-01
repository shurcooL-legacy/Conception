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

// DEBUG: Values changed for testing
const double TapTime = 10.25;
const double DoubleTapTime = 2 * 0.25;//TapTime;

uint8 MatchDoubleTap(const InputEventQueue::Queue & Queue, InputEventQueue::Queue::const_iterator InputEventIterator, InputEventQueue::Queue::const_iterator & InputEventIteratorEnd)
{
	if (   Queue.end() != InputEventIterator
		&& nullptr != InputEventIterator->m_Pointer
		&& Pointer::VirtualCategory::POINTING == InputEventIterator->m_Pointer->GetVirtualCategory()
		&& InputEventIterator->HasType(InputEvent::EventType::BUTTON_EVENT)
		&& 0 == InputEventIterator->m_InputId
		&& true == InputEventIterator->m_Buttons[0])
	{
		Vector2n DownPosition(InputEventIterator->m_PreEventState.GetAxisState(0).GetPosition(), InputEventIterator->m_PreEventState.GetAxisState(1).GetPosition());
		auto DownTime = InputEventIterator->GetTimestamp();

		uint8 NumberOfTaps = 0;

		auto InputEventIterator2 = InputEventIterator;
		++InputEventIterator2;
		for (; Queue.end() != InputEventIterator2; ++InputEventIterator2)
		{
			if (   nullptr != InputEventIterator2->m_Pointer
				&& Pointer::VirtualCategory::POINTING == InputEventIterator2->m_Pointer->GetVirtualCategory()
				&& InputEventIterator2->HasType(InputEvent::EventType::BUTTON_EVENT)
				&& 0 == InputEventIterator2->m_InputId
				&& false == InputEventIterator2->m_Buttons[0])
			{
				Vector2n UpPosition(InputEventIterator2->m_PreEventState.GetAxisState(0).GetPosition(), InputEventIterator2->m_PreEventState.GetAxisState(1).GetPosition());
				auto UpTime = InputEventIterator2->GetTimestamp();

				if (   (UpPosition - DownPosition).LengthSquared() <= (3 * 3)
					&& (UpTime - DownTime) <= DoubleTapTime)
				{
					if (1 == NumberOfTaps)
					{
						++InputEventIterator2;
						InputEventIteratorEnd = InputEventIterator2;
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
			else if (   nullptr != InputEventIterator2->m_Pointer
					 && Pointer::VirtualCategory::POINTING == InputEventIterator2->m_Pointer->GetVirtualCategory()
					 && (   InputEventIterator2->HasType(InputEvent::EventType::AXIS_EVENT)
						 || InputEventIterator2->HasType(InputEvent::EventType::CANVAS_MOVED_TEST))
					 && 0 == InputEventIterator2->m_InputId)
			{
				Vector2n NewPosition(InputEventIterator2->m_PreEventState.GetAxisState(0).GetPosition(), InputEventIterator2->m_PreEventState.GetAxisState(1).GetPosition());

				if ((NewPosition - DownPosition).LengthSquared() <= (3 * 3))
				{
				}
				else
				{
					return 0;
				}
			}
			else if (   nullptr != InputEventIterator2->m_Pointer
					 && Pointer::VirtualCategory::POINTING == InputEventIterator2->m_Pointer->GetVirtualCategory()
					 && InputEventIterator2->HasType(InputEvent::EventType::BUTTON_EVENT)
					 && 0 == InputEventIterator2->m_InputId
					 && true == InputEventIterator2->m_Buttons[0])
			{}
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

uint8 MatchTap(const InputEventQueue::Queue & Queue, InputEventQueue::Queue::const_iterator InputEventIterator, InputEventQueue::Queue::const_iterator & InputEventIteratorEnd)
{
	if (   Queue.end() != InputEventIterator
		&& nullptr != InputEventIterator->m_Pointer
		&& Pointer::VirtualCategory::POINTING == InputEventIterator->m_Pointer->GetVirtualCategory()
		&& InputEventIterator->HasType(InputEvent::EventType::BUTTON_EVENT)
		&& 0 == InputEventIterator->m_InputId
		&& true == InputEventIterator->m_Buttons[0])
	{
		Vector2n DownPosition(InputEventIterator->m_PreEventState.GetAxisState(0).GetPosition(), InputEventIterator->m_PreEventState.GetAxisState(1).GetPosition());
		auto DownTime = InputEventIterator->GetTimestamp();

		auto InputEventIterator2 = InputEventIterator;
		++InputEventIterator2;
		for (; Queue.end() != InputEventIterator2; ++InputEventIterator2)
		{
			if (   nullptr != InputEventIterator2->m_Pointer
				&& Pointer::VirtualCategory::POINTING == InputEventIterator2->m_Pointer->GetVirtualCategory()
				&& InputEventIterator2->HasType(InputEvent::EventType::BUTTON_EVENT)
				&& 0 == InputEventIterator2->m_InputId
				&& false == InputEventIterator2->m_Buttons[0])
			{
				Vector2n UpPosition(InputEventIterator2->m_PreEventState.GetAxisState(0).GetPosition(), InputEventIterator2->m_PreEventState.GetAxisState(1).GetPosition());
				auto UpTime = InputEventIterator2->GetTimestamp();

				if (   (UpPosition - DownPosition).LengthSquared() <= (3 * 3)
					&& (UpTime - DownTime) <= TapTime)
				{
					++InputEventIterator2;
					InputEventIteratorEnd = InputEventIterator2;
					return 2;
				}
				else
				{
					return 0;
				}
			}
			else if (   nullptr != InputEventIterator2->m_Pointer
					 && Pointer::VirtualCategory::POINTING == InputEventIterator2->m_Pointer->GetVirtualCategory()
					 && (   InputEventIterator2->HasType(InputEvent::EventType::AXIS_EVENT)
						 || InputEventIterator2->HasType(InputEvent::EventType::CANVAS_MOVED_TEST))
					 && 0 == InputEventIterator2->m_InputId)
			{
				Vector2n NewPosition(InputEventIterator2->m_PreEventState.GetAxisState(0).GetPosition(), InputEventIterator2->m_PreEventState.GetAxisState(1).GetPosition());

				if ((NewPosition - DownPosition).LengthSquared() <= (3 * 3))
				{
				}
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

uint8 MatchDown(const InputEventQueue::Queue & Queue, InputEventQueue::Queue::const_iterator InputEventIterator, InputEventQueue::Queue::const_iterator & InputEventIteratorEnd)
{
	if (   Queue.end() != InputEventIterator
		&& nullptr != InputEventIterator->m_Pointer
		&& Pointer::VirtualCategory::POINTING == InputEventIterator->m_Pointer->GetVirtualCategory()
		&& InputEventIterator->HasType(InputEvent::EventType::BUTTON_EVENT)
		&& 0 == InputEventIterator->m_InputId
		&& true == InputEventIterator->m_Buttons[0])
	{
		++InputEventIteratorEnd;

		return 2;
	}

	return 0;
}

uint8 MatchUp(const InputEventQueue::Queue & Queue, InputEventQueue::Queue::const_iterator InputEventIterator, InputEventQueue::Queue::const_iterator & InputEventIteratorEnd)
{
	if (   Queue.end() != InputEventIterator
		&& nullptr != InputEventIterator->m_Pointer
		&& Pointer::VirtualCategory::POINTING == InputEventIterator->m_Pointer->GetVirtualCategory()
		&& InputEventIterator->HasType(InputEvent::EventType::BUTTON_EVENT)
		&& 0 == InputEventIterator->m_InputId
		&& false == InputEventIterator->m_Buttons[0])
	{
		++InputEventIteratorEnd;

		return 2;
	}

	return 0;
}

void App::ProcessEventQueue(InputEventQueue & InputEventQueue)
{
//return;

	// Pass it through widgets

	{
		//for (auto & InputEvent : InputEventQueue.Queue())
		for (auto InputEventIterator = InputEventQueue.GetQueue().begin(); InputEventQueue.GetQueue().end() != InputEventIterator; )
		//while (!InputEventQueue.Queue().empty())
		{
			//auto InputEventIterator = InputEventQueue.Queue().begin();

			//std::cout << "1st loop " << InputEvent.ToString() << std::endl;

#if 0
			// TODO: Fix bug where dragging the object out of down-event range and then bringing it back still activates the tap (it shouldn't)
			if (   m_RecognizeTap
				&& !InputEvent.m_Handled)
			{
				if (   InputEvent.HasType(InputEvent::EventType::BUTTON_EVENT)
					&& 0 == InputEvent.m_InputId
					&& true == InputEvent.m_Buttons[0])
				{
					m_LastTapStateTEST = InputEvent.m_Pointer->GetPointerState();
				}
				else if (   InputEvent.HasType(InputEvent::EventType::BUTTON_EVENT)
						 && 0 == InputEvent.m_InputId
						 && false == InputEvent.m_Buttons[0]
						 //&& std::fabs(InputEvent.m_Timestamp - m_LastTapEventTEST.m_Timestamp) <= 1.0)
						 && (Vector2n(InputEvent.m_Pointer->GetPointerState().GetAxisState(0).GetPosition(), InputEvent.m_Pointer->GetPointerState().GetAxisState(1).GetPosition()) - Vector2n(m_LastTapStateTEST.GetAxisState(0).GetPosition(), m_LastTapStateTEST.GetAxisState(1).GetPosition())).LengthSquared() <= (3 * 3))
				{
					printf("Recognized a tap.\n");
					InputEvent.m_Handled = true;
					m_Owner.ProcessTap(InputEvent, Vector2n(m_LastTapStateTEST.GetAxisState(0).GetPosition(), m_LastTapStateTEST.GetAxisState(1).GetPosition()));
					m_LastTapCompletedStateTEST = m_LastTapStateTEST;
				}
			}
#endif

			auto InputEventIterator2 = InputEventIterator;
			auto Status = MatchDoubleTap(InputEventQueue.GetQueue(), InputEventIterator, InputEventIterator2);
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
			}

			InputEventIterator2 = InputEventIterator;
			Status = MatchTap(InputEventQueue.GetQueue(), InputEventIterator, InputEventIterator2);
			if (2 == Status)
			{
				std::cout << "Tap at " << InputEventIterator->m_Pointer->GetPointerState().GetAxisState(0).GetPosition() << ","
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
			}

			/*InputEventIterator2 = InputEventIterator;
			Status = MatchDown(InputEventQueue.GetQueue(), InputEventIterator, InputEventIterator2);
			if (2 == Status)
			{
				std::cout << "Mouse Down at " << InputEventIterator->m_Pointer->GetPointerState().GetAxisState(0).GetPosition() << "," \
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
			}

			InputEventIterator2 = InputEventIterator;
			Status = MatchUp(InputEventQueue.GetQueue(), InputEventIterator, InputEventIterator2);
			if (2 == Status)
			{
				std::cout << "Mouse Up at " << InputEventIterator->m_Pointer->GetPointerState().GetAxisState(0).GetPosition() << "," \
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

		//InputEventQueue.ModifyQueue().clear();
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
