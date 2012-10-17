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

void App::ProcessEvent(InputEvent & InputEvent)
{
	if (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::AXIS_EVENT)
		|| InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::CANVAS_MOVED_TEST))
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

	//return true;
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
			else if (dynamic_cast<ListWidget<ConceptId> *>(&i->GetOwner())) out << "\n ListWidget<ConceptId>";

			auto LocalPosition = dynamic_cast<Widget *>(&i->GetOwner())->GlobalToLocal(Vector2n(g_InputManager->m_MousePointer->GetPointerState().GetAxisState(0).GetPosition(), g_InputManager->m_MousePointer->GetPointerState().GetAxisState(1).GetPosition()));
			out << " (" << LocalPosition.X() << ", " << LocalPosition.Y() << ")";
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
