#include "Main.h"

App::App(InputManager & InputManager)
	: InputResponder(),
	  m_Widgets(),
	  m_InputManager(InputManager)
{
	ModifyInputManager().SetInputResponder(this);
}

App::~App()
{
	printf("~App()\n");

	ModifyInputManager().ClearInputResponder();
}

void App::Layout()
{
	{
		InputEvent InputEvent;
		InputEvent.m_EventTypes.insert(InputEvent::EventType::PARENT_SIZE);		// TODO: Maybe put the event details inside?

		ProcessEvent(InputEvent);
	}
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

#if 0
void App::ProcessPosition(Pointer * Pointer, Vector2n Position)
{
	if (nullptr == Pointer->GetWidgetMapping().GetActiveWidget())
	{
		Pointer->GetWidgetMapping().RemoveHoverPointer(Pointer);		// TODO: Maybe only remove/add when there's change, rather than starting from scratch each time

		std::list<Widget *> Hits;		// Front of list are top-most widgets

		//for (auto & Widget : m_Widgets)
		for (auto Widget = m_Widgets.rbegin(); m_Widgets.rend() != Widget; ++Widget)
		{
			if ((*Widget)->HitTest(Position, &Hits))
				break;		// HACK: I need to do proper query if the HitTest prevents passthrough interest check
		}

		/*std::cout << std::string(100, '\n');
		for (auto & Hit : Hits)
		{
			std::cout << "Hit widget " << Hit << std::endl;
		}*/
		/*OpenGLStream out(Vector2n(0, 0));
		for (auto & Hit : Hits)
		{
			std::stringstream ss;
			ss << "Hit widget " << Hit;
			out << ss.str() << endl;
		}*/

		Pointer->GetWidgetMapping().RemoveAllMappings();
		for (auto & Hit : Hits)
		{
			Pointer->GetWidgetMapping().AddMapping(Hit);
			Hit->AddHoverPointer(Pointer);
		}
		Pointer->GetWidgetMapping().DoneAdding();
	}
}
#endif

bool App::ProcessEvent(InputEvent & InputEvent)
{
#if 0
	if (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::POINTER_ACTIVATION)
		&& 0 == InputEvent.m_DeviceCategory)
	{
		// TEST: Pick the first widget (main canvas)
		Widget * Hit = m_Widgets.front().get();

		if (nullptr == Hit->m_GestureRecognizerTEST)
		{
			//printf("Created a new Gesture Recognizer\n");
			Hit->m_GestureRecognizerTEST = std::unique_ptr<GestureRecognizer>(new GestureRecognizer(*Hit));
			Hit->m_GestureRecognizerTEST->m_RecognizeTap = true;
		}
		InputEvent.m_Pointer->m_GestureRecognizerTEST = Hit->m_GestureRecognizerTEST.get();
		MutuallyAttachable<Pointer, GestureRecognizer>::CreateConnection(*InputEvent.m_Pointer, *InputEvent.m_Pointer->m_GestureRecognizerTEST);
	}

	if (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::POINTER_ACTIVATION)
		&& (   1 == InputEvent.m_DeviceCategory
			|| 2 == InputEvent.m_DeviceCategory))
	{
		if (nullptr != InputEvent.m_Pointer)
		{
			std::list<Widget *> Hits;		// Front of list are top-most widgets

			//for (auto & Widget : m_Widgets)
			for (auto Widget = m_Widgets.rbegin(); m_Widgets.rend() != Widget; ++Widget)
			{
				if ((*Widget)->HitTest(Vector2n(InputEvent.GetPointerState().GetAxisState(0).GetPosition(), InputEvent.GetPointerState().GetAxisState(1).GetPosition()), &Hits))
					break;		// HACK: I need to do proper query if the HitTest prevents passthrough interest check
			}

			//Pointer->GetWidgetMapping().RemoveAllMappings();
			for (auto & Hit : Hits)
			{
				//Pointer->GetWidgetMapping().AddMapping(Hit);
				//Hit->AddHoverPointer(Pointer);
				
				if (nullptr == Hit->m_GestureRecognizerTEST)
				{
					//printf("Created a new Gesture Recognizer\n");
					Hit->m_GestureRecognizerTEST = std::unique_ptr<GestureRecognizer>(new GestureRecognizer(*Hit));
					Hit->m_GestureRecognizerTEST->m_RecognizeTap = true;
				}
				InputEvent.m_Pointer->m_GestureRecognizerTEST = Hit->m_GestureRecognizerTEST.get();
				MutuallyAttachable<Pointer, GestureRecognizer>::CreateConnection(*InputEvent.m_Pointer, *InputEvent.m_Pointer->m_GestureRecognizerTEST);
				break;
			}
			//Pointer->GetWidgetMapping().DoneAdding();
		}
	}

	if (   nullptr != InputEvent.m_Pointer
		&& nullptr != InputEvent.m_Pointer->m_GestureRecognizerTEST)
	{
		InputEvent.m_Pointer->m_GestureRecognizerTEST->ProcessEvent(InputEvent);
	}

	if (InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::POINTER_DEACTIVATION))
	{
		if (   nullptr != InputEvent.m_Pointer
			&& nullptr != InputEvent.m_Pointer->m_GestureRecognizerTEST)
		{
			bool LastGone = false;
			if (InputEvent.m_Pointer->m_GestureRecognizerTEST->GetAttached().size() <= 1)
			{
				LastGone = true;
			}

			MutuallyAttachable<Pointer, GestureRecognizer>::DestroyConnection(*InputEvent.m_Pointer, *InputEvent.m_Pointer->m_GestureRecognizerTEST);

			if (LastGone)
			{
				static_cast<Widget *>(&InputEvent.m_Pointer->m_GestureRecognizerTEST->GetOwner())->m_GestureRecognizerTEST.reset();
				if (InputEvent.m_Pointer->m_GestureRecognizerTEST == g_InputManager->m_MousePointer->m_GestureRecognizerTEST)		// HACK
					g_InputManager->m_MousePointer->m_GestureRecognizerTEST = nullptr;
				if (InputEvent.m_Pointer->m_GestureRecognizerTEST == g_InputManager->m_TypingPointer->m_GestureRecognizerTEST)		// HACK
					g_InputManager->m_TypingPointer->m_GestureRecognizerTEST = nullptr;
				InputEvent.m_Pointer->m_GestureRecognizerTEST = nullptr;
			}
		}
	}
#endif

	// DEBUG: Keyboard arrow keys move thingy for main canvas
	/*if (InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::POINTER_ACTIVATION))
	{
		if (0 == InputEvent.m_DeviceCategory)
		{
			MutuallyConnectable<Pointer, GestureRecognizer>::Connect(*g_InputManager->m_TypingPointer, m_Widgets.front()->ModifyGestureRecognizer());
			printf("Connected.\n");
		}
		else
		{
		}
	}

	if (InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::POINTER_DEACTIVATION))
	{
		if (0 == InputEvent.m_DeviceCategory)
		{
			MutuallyConnectable<Pointer, GestureRecognizer>::Disconnect(*g_InputManager->m_TypingPointer, m_Widgets.front()->ModifyGestureRecognizer());
			printf("Disconnected.\n");
		}
		else
		{
		}
	}*/

	if (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::AXIS_EVENT)
		|| InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::CANVAS_MOVED_TEST))
	{
		if (Pointer::VirtualCategory::POINTING == InputEvent.m_Pointer->GetVirtualCategory())
		{
			if (nullptr == InputEvent.m_Pointer->GetPointerMapping().GetCapturer())
			{
				//Pointer->GetPointerMapping().RemoveHoverPointer(Pointer);		// TODO: Maybe only remove/add when there's change, rather than starting from scratch each time

				std::list<Widget *> Hits;		// Front of list are top-most widgets

				//for (auto & Widget : m_Widgets)
				for (auto Widget = m_Widgets.rbegin(); m_Widgets.rend() != Widget; ++Widget)
				{
					Vector2n GlobalPosition(InputEvent.m_Pointer->GetPointerState().GetAxisState(0).GetPosition(), InputEvent.m_Pointer->GetPointerState().GetAxisState(1).GetPosition());

					auto Result = (*Widget)->HitTest(GlobalPosition, &Hits);
#if DECISION_POINTER_MAPPING_CONTAINS_SINGLE_TOPMOST_WIDGET
					if (Result)
						break;		// HACK: I need to do proper query if the HitTest prevents passthrough interest check
#endif
				}

				/*std::cout << std::string(100, '\n');
				for (auto & Hit : Hits)
				{
					std::cout << "Hit widget " << Hit << std::endl;
				}*/
				/*OpenGLStream out(Vector2n(0, 0));
				for (auto & Hit : Hits)
				{
					std::stringstream ss;
					ss << "Hit widget " << Hit;
					out << ss.str() << endl;
				}*/

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

	if (InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::PARENT_SIZE))
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
		if (InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::BUTTON_EVENT))
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
						glfwCloseWindow();
						break;
					// TEST
					case 'M':
						//if (glfwGetKey(GLFW_KEY_LCTRL) || glfwGetKey(GLFW_KEY_RCTRL))
						{
							//g_InputManager->SetMouseCursorVisibility(!g_InputManager->IsMouseCursorVisible());
							/*int xpos, ypos;
							glfwGetMousePos(&xpos, &ypos);
							glfwSetMousePos(xpos+1, ypos);*/
						}
						break;
					default:
						break;
					}
				}
			}
		}
	}

	return true;
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

		out << "Mouse.PntrMppng.m_Entries.size(): " << g_InputManager->m_MousePointer->ModifyPointerMapping().m_Entries.size();
		for (auto & i : g_InputManager->m_MousePointer->ModifyPointerMapping().m_Entries)
		{
			if (dynamic_cast<Canvas *>(&i->GetOwner())) out << "\n Canvas";
			else if (dynamic_cast<MultitouchTestBoxWidget *>(&i->GetOwner())) out << "\n MultitouchTestBoxWidget, color: " << static_cast<uint16>(static_cast<MultitouchTestBoxWidget *>(&i->GetOwner())->m_Color);
			else if (dynamic_cast<TextFieldWidget *>(&i->GetOwner())) out << "\n TextFieldWidget";
			else if (dynamic_cast<ButtonWidget *>(&i->GetOwner())) out << "\n ButtonWidget";
			else if (dynamic_cast<ListWidget *>(&i->GetOwner())) out << "\n ListWidget";

			auto LocalPosition = dynamic_cast<Widget *>(&i->GetOwner())->GlobalToLocal(Vector2n(g_InputManager->m_MousePointer->GetPointerState().GetAxisState(0).GetPosition(), g_InputManager->m_MousePointer->GetPointerState().GetAxisState(1).GetPosition()));
			out << " (" << LocalPosition.X() << ", " << LocalPosition.Y() << ")";
		}

		OpenGLStream(Vector2n::ZERO) << out.str();
	}
}
