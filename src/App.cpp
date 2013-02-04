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

void App::UpdateWindowDimensions(Vector2n WindowDimensions)
{
	// TODO: This may still be a hack, I should create a WindowResize listener type of thing and take care within Widget itself, or perhaps something else?
	for (auto & Widget : m_Widgets)
	{
		if (nullptr != dynamic_cast<Canvas *>(Widget.get()))
		{
			Widget->SetDimensions(WindowDimensions);
		}
	}
}

/*MatchResult ExternalMatchingTEST(InputEventQueue::FilteredQueue & UnreservedEvents)
{
	auto InputEventIterator = UnreservedEvents.begin();
	auto & InputEvent = **InputEventIterator;

	MatchResult Match;
	if ((Match = MatchSpace(UnreservedEvents, InputEventIterator)).AnySuccess())
	{
		if (2 == Match.Status)
		{
			std::cout << "Space in ExternalMatchingTEST!" << std::endl;
		}
	}
	else if (IsPointerButtonEvent<Pointer::VirtualCategory::TYPING, 'M', true>(InputEvent))
	{
		std::cout << "M in ExternalMatchingTEST!" << std::endl;

		Match.Status = 2;
		Match.Events.push_back(*InputEventIterator);
	}
	else if ((Match = MatchTap2(UnreservedEvents, InputEventIterator)).AnySuccess())
	{
		if (2 == Match.Status)
		{
			std::cout << "Tap EXTERNAL at " << (*InputEventIterator)->m_PostEventState.GetAxisState(0).GetPosition() << "," \
											<< (*InputEventIterator)->m_PostEventState.GetAxisState(1).GetPosition() << std::endl;
		}
	}

	return Match;
}*/

MatchResult App::MatchEventQueue(InputEventQueue::FilteredQueue & UnreservedEvents)
{
	for (auto & Widget : reverse(m_Widgets))
	{
		auto Match = Widget->MatchEventQueue(UnreservedEvents);

		if (Match.AnySuccess())
			return Match;
	}

	return MatchResult();
}

void App::ProcessEventQueue(InputEventQueue & InputEventQueue)
{
	if (0)
	while (!InputEventQueue.GetQueue().empty())
	{
		ProcessEvent(*(InputEventQueue.ModifyQueue().begin()));

		InputEventQueue.ModifyQueue().pop_front();
	}

	// Pass it through widgets
	if (0)
	while (!InputEventQueue.GetQueue().empty())
	{
		auto & InputEvent = *(InputEventQueue.ModifyQueue().begin());

		{
			if (   InputEvent.HasType(InputEvent::EventType::AXIS_EVENT)
				|| InputEvent.HasType(InputEvent::EventType::CANVAS_MOVED_TEST))
			{
				if (Pointer::VirtualCategory::POINTING == InputEvent.m_Pointer->GetVirtualCategory())
				{
					if (nullptr == InputEvent.m_Pointer->GetPointerMapping().GetCapturer())
					{
						std::list<Widget *> Hits;		// Front of list are top-most widgets

						for (auto & Widget : reverse(m_Widgets))
						{
							Vector2n GlobalPosition(InputEvent.m_PostEventState.GetAxisState(0).GetPosition(), InputEvent.m_PostEventState.GetAxisState(1).GetPosition());

							auto Result = Widget->HitTest(GlobalPosition, &Hits);
#if DECISION_POINTER_MAPPING_CONTAINS_SINGLE_TOPMOST_WIDGET
							if (Result)
								break;		// HACK: I need to do proper query if the HitTest prevents passthrough interest check
#endif
						}

						InputEvent.m_Pointer->ModifyPointerMapping().RemoveAllMappings();		// TODO: Maybe only remove/add when there's change, rather than starting from scratch each time
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

		InputEventQueue.ModifyQueue().pop_front();
	}

	// TEST: Gesture recognition and handling testing
	if (1)
	{
		static bool InManipulationTEST = false;

		auto UnreservedEvents = InputEventQueue.CreateFilteredQueue();

		//for (auto & InputEvent : InputEventQueue.Queue())
		//for (auto InputEventIterator = InputEventQueue.GetQueue().begin(); InputEventQueue.GetQueue().end() != InputEventIterator; )
		while (!UnreservedEvents.empty())
		{
			auto InputEventIterator = UnreservedEvents.begin();

			//decltype(InputEventIterator) InputEventIterator2;
			//uint8 Status;

			auto & InputEvent = **InputEventIterator;

			// DECISION
#if 0
			// Release the pointer capture if the pointer is deactivated
			if (InputEvent.HasType(InputEvent::EventType::POINTER_DEACTIVATION))
			{
				InputEvent.m_Pointer->ModifyPointerMapping().RequestPointerRelease(reinterpret_cast<GestureRecognizer *>(1));
			}
#endif

#if 1
			// Populate PointerMappings
			{
				if (   IsPointerPointingMoveEvent<0>(InputEvent)
					|| IsPointerPointingDeactivationEvent(InputEvent))
				{
					//if (nullptr == InputEvent.m_Pointer->GetPointerMapping().GetCapturer())
					if (false == InputEvent.m_Pointer->IsActive())
					{
						std::list<Widget *> Hits;		// Front of list are top-most widgets

						for (auto & Widget : reverse(m_Widgets))
						{
							Vector2n GlobalPosition(InputEvent.m_PostEventState.GetAxisState(0).GetPosition(), InputEvent.m_PostEventState.GetAxisState(1).GetPosition());

							auto Result = Widget->HitTest(GlobalPosition, &Hits);
#if DECISION_POINTER_MAPPING_CONTAINS_SINGLE_TOPMOST_WIDGET
							if (Result)
								break;		// HACK: I need to do proper query if the HitTest prevents passthrough interest check
#endif
						}

						InputEvent.m_Pointer->ModifyPointerMapping().RemoveAllMappings();		// TODO: Maybe only remove/add when there's change, rather than starting from scratch each time
						for (auto & Hit : Hits)
						{
							InputEvent.m_Pointer->ModifyPointerMapping().AddMapping(Hit->ModifyGestureRecognizer());
						}
						InputEvent.m_Pointer->ModifyPointerMapping().DoneAdding();
					}
				}
			}
#endif

			// DECISION
#if 0
			// TEST: Capture the pointer if the pointer is activated //(via primary button)
			if (   InputEvent.HasType(InputEvent::EventType::POINTER_ACTIVATION)
				/*&& (   InputEvent.HasType(InputEvent::EventType::BUTTON_EVENT)
					&& 0 == InputEvent.m_InputId
					&& true == InputEvent.m_Buttons[0])*/)
			{
				InputEvent.m_Pointer->ModifyPointerMapping().RequestPointerCapture(reinterpret_cast<GestureRecognizer *>(1));
			}
#endif

			MatchResult Match;
			if (InputEvent.HasType(InputEvent::EventType::PARENT_SIZE))
			{
				UpdateWindowDimensions(ModifyInputManager().GetWindowDimensions());

				Match.Status = 2;
				Match.Events.push_back(*InputEventIterator);
			}

			// Waterfall through GRs of Widgets
			else if ((Match = MatchEventQueue(UnreservedEvents)).AnySuccess())
			{
			}

			// TEST
			/*else if ((Match = ExternalMatchingTEST(UnreservedEvents)).AnySuccess())
			{
			}*/

			// DEBUG, TEST: System key handling
			else if (IsPointerButtonEvent<Pointer::VirtualCategory::TYPING, GLFW_KEY_ESC, true>(InputEvent))
			{
				glfwCloseWindow();

				Match.Status = 2;
				Match.Events.push_back(*InputEventIterator);
			}
			else if (IsPointerButtonEvent<Pointer::VirtualCategory::TYPING, 'M', true>(InputEvent))
			{
				/*//if (glfwGetKey(GLFW_KEY_LCTRL) || glfwGetKey(GLFW_KEY_RCTRL))
				{
					//g_InputManager->SetMouseCursorVisibility(!g_InputManager->IsMouseCursorVisible());
					int xpos, ypos;
					glfwGetMousePos(&xpos, &ypos);
					glfwSetMousePos(xpos+1, ypos);
				}*/
				std::cout << "M was pressed for some reason..." << std::endl;

				Match.Status = 2;
				Match.Events.push_back(*InputEventIterator);
			}
			else if (	IsPointerButtonEvent<Pointer::VirtualCategory::TYPING, 'R', true>(InputEvent)
					 && PointerState::Modifiers::Super == InputEvent.m_PostEventState.GetModifiers())
			{
				std::cout << "Cmd+R was pressed in App..." << std::endl;

				Match.Status = 2;
				Match.Events.push_back(*InputEventIterator);
			}

			/*auto InputEventIterator2 = InputEventIterator;
			auto Status = MatchDoubleTap2(InputEventQueue.GetQueue(), InputEventIterator2);
			if (2 == Status)
			{
				std::cout << "Double Tap at " << InputEventIterator->m_PostEventState.GetAxisState(0).GetPosition() << ","
											  << InputEventIterator->m_PostEventState.GetAxisState(1).GetPosition() << std::endl;

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

			else if ((Match = MatchTap2(UnreservedEvents, InputEventIterator)).AnySuccess())
			{
				if (2 == Match.Status)
				{
					std::cout << "Tap at " << (*InputEventIterator)->m_PostEventState.GetAxisState(0).GetPosition() << "," \
										   << (*InputEventIterator)->m_PostEventState.GetAxisState(1).GetPosition() << std::endl;
				}
			}

			else if ((Match = MatchManipulationBegin(UnreservedEvents, InputEventIterator, InManipulationTEST, true)).AnySuccess())
			{
				if (2 == Match.Status)
				{
					std::cout << "ManipulationBegin at " << (*InputEventIterator)->m_PostEventState.GetAxisState(0).GetPosition() << "," \
														 << (*InputEventIterator)->m_PostEventState.GetAxisState(1).GetPosition() << std::endl;
					InManipulationTEST = true;
				}
			}
			else if ((Match = MatchManipulationUpdate(UnreservedEvents, InputEventIterator, InManipulationTEST)).AnySuccess())
			{
				if (2 == Match.Status)
				{
					//std::cout << "ManipulationUpdate at " << (*InputEventIterator)->m_PostEventState.GetAxisState(0).GetPosition() << "," \
														  << (*InputEventIterator)->m_PostEventState.GetAxisState(1).GetPosition() << std::endl;
				}
			}
			else if ((Match = MatchManipulationEnd(UnreservedEvents, InputEventIterator, InManipulationTEST)).AnySuccess())
			{
				if (2 == Match.Status)
				{
					std::cout << "ManipulationEnd at " << (*InputEventIterator)->m_PostEventState.GetAxisState(0).GetPosition() << "," \
													   << (*InputEventIterator)->m_PostEventState.GetAxisState(1).GetPosition() << std::endl;
					InManipulationTEST = false;
				}
			}

			else if ((Match = MatchDown(UnreservedEvents, InputEventIterator)).AnySuccess())
			{
				if (2 == Match.Status)
				{
					std::cout << "Mouse Down at " << (*InputEventIterator)->m_PostEventState.GetAxisState(0).GetPosition() << "," \
												  << (*InputEventIterator)->m_PostEventState.GetAxisState(1).GetPosition() << std::endl;
				}
			}

			else if ((Match = MatchUp(UnreservedEvents, InputEventIterator)).AnySuccess())
			{
				if (2 == Match.Status)
				{
					std::cout << "Mouse Up at " << (*InputEventIterator)->m_PostEventState.GetAxisState(0).GetPosition() << "," \
												<< (*InputEventIterator)->m_PostEventState.GetAxisState(1).GetPosition() << std::endl;
				}
			}

			else if ((Match = MatchSpace(UnreservedEvents, InputEventIterator)).AnySuccess())
			{
				if (2 == Match.Status)
				{
					std::cout << "Space" << std::endl;
				}
			}

			else
			{
				// Nothing matched this InputEvent at all, so delete it
				Match.Status = 2;
				Match.Events.push_back(*InputEventIterator);
			}

			if (Match.AnySuccess())
			{
				InputEventQueue::EraseEventsFromFilteredQueue(UnreservedEvents, Match.Events);

				if (2 == Match.Status)
					InputEventQueue.EraseEventsFromQueue(Match.Events);
			}
		}

		/*for (auto & InputEventIterator : InputEventQueue::FilterByPointer(InputEventQueue.CreateFilteredQueue(), g_InputManager->m_MousePointer.get()))
		{
			std::cout << "2nd loop " << InputEventIterator->ToString() << std::endl;

			if (   InputEventIterator->HasType(InputEvent::EventType::BUTTON_EVENT)
				&& 0 == InputEventIterator->m_InputId
				&& true == InputEventIterator->m_Buttons[0])
			{
				std::cout << "Mouse Down at " << InputEventIterator->m_PostEventState.GetAxisState(0).GetPosition() << ","
											  << InputEventIterator->m_PostEventState.GetAxisState(1).GetPosition() << std::endl;
			}

			if (   InputEventIterator->HasType(InputEvent::EventType::BUTTON_EVENT)
				&& 0 == InputEventIterator->m_InputId
				&& false == InputEventIterator->m_Buttons[0])
			{
				std::cout << "Mouse Up at " << InputEventIterator->m_PostEventState.GetAxisState(0).GetPosition() << ","
											<< InputEventIterator->m_PostEventState.GetAxisState(1).GetPosition() << std::endl;
			}
		}*/
	}
}

#if 0
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
#endif

void App::ProcessTimePassed(const double TimePassed)
{
	for (auto & Widget : m_Widgets)
	{
		Widget->ProcessTimePassed(TimePassed);
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
