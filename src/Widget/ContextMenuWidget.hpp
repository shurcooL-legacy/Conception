template <typename T> ContextMenuWidget<T>::ContextMenuWidget(Vector2n Position, std::vector<T> & Entries)
	: Widget(Position, Vector2n::ZERO, {}),
	  m_TapAction(),
	  m_Entries(Entries),
	  m_SelectedEntry(0)
{
	ModifyGestureRecognizer().m_RecognizeTap = true;
	ModifyGestureRecognizer().m_RecognizeDoubleTap = true;
	ModifyGestureRecognizer().m_RecognizeManipulationTranslate = false;

	UpdateDimensions();
}

template <typename T> ContextMenuWidget<T>::~ContextMenuWidget()
{
}

template <typename T> const T * ContextMenuWidget<T>::GetSelectedEntry() const
{
	if (!m_Entries.empty())
		return &m_Entries[m_SelectedEntry];
	else
		return nullptr;
}

template <typename T> void ContextMenuWidget<T>::Render()
{
	Color BackgroundColor(1.0, 1.0, 1.0);
	Color BorderColor(0.3, 0.3, 0.3);

	/*if (CheckHover(WidgetManager) && CheckActive(WidgetManager))
	{
	}
	else if ((CheckHover(WidgetManager) && !CheckAnyActive(WidgetManager)) || (!CheckHover(WidgetManager) && CheckActive(WidgetManager)))
	{
		BorderColor[0] = 0.898;
		BorderColor[1] = 0.765;
		BorderColor[2] = 0.396;
	}
	else
	{
	}*/

	// TODO: Think if I should outsource the entire rendering code for more generality
	// Draw list
	{
		UpdateDimensions();		// LATER: Optimize by not repeating this calculation each time, only when something changes?

		if (m_Entries.empty())
		{
			BackgroundColor[0] = 234 / 255.0;
			BackgroundColor[1] = 233 / 255.0;
			BackgroundColor[2] = 190 / 255.0;
		}

		DrawAroundBox(GetPosition(), GetDimensions(), BackgroundColor, BorderColor);

		OpenGLStream OpenGLStream(GetPosition());
		//for (auto & Entry : m_Entries)
		for (auto Entry = m_Entries.begin(); m_Entries.end() != Entry; ++Entry)
		{
			if (Entry - m_Entries.begin() == m_SelectedEntry)
			{
				DrawBox(GetPosition() + Vector2n(0, static_cast<sint32>((Entry - m_Entries.begin()) * lineHeight)), Vector2n(GetDimensions().X(), lineHeight), m_SelectedColor, m_SelectedColor);
			}

			OpenGLStream << *Entry << endl;
		}
	}
}

template <typename T> void ContextMenuWidget<T>::ProcessTap(InputEvent & InputEvent, Vector2n Position)
{
	if (nullptr != m_TapAction)
	{
		m_TapAction(GlobalToLocal(Position), m_Entries);
	}
}

template <typename T> void ContextMenuWidget<T>::ProcessDoubleTap(InputEvent & InputEvent, Vector2n Position)
{
	// TODO: Do something on double-tap, like insert the selected autocompletion into parent

	g_InputManager->RequestTypingPointer(ModifyParent()->ModifyGestureRecognizer());
	InputEvent.m_Pointer->ModifyPointerMapping().RemoveMapping(ModifyGestureRecognizer());
	ModifyParent()->RemoveWidget(this);		// TODO: Maybe this is wrong to do, verify
	InputEvent.m_Handled = true;		// TOOD: I can't even remember if I need to do this? Should do this? Does it make any difference... *sigh*
}

template <typename T> void ContextMenuWidget<T>::ProcessEvent(InputEvent & InputEvent)
{
	if (InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::BUTTON_EVENT))
	{
		auto ButtonId = InputEvent.m_InputId;
		bool Pressed = InputEvent.m_Buttons[0];		// TODO: Check if there are >1 buttons

		if (Pointer::VirtualCategory::TYPING == InputEvent.m_Pointer->GetVirtualCategory())
		{
			if (Pressed)
			{
				bool HandledEvent = true;		// Assume true at first

				switch (ButtonId)
				{
				case GLFW_KEY_ESC:
					{
						ModifyParent()->RemoveWidget(this);
						g_InputManager->RequestTypingPointer(ModifyParent()->ModifyGestureRecognizer());
					}
					break;
				case GLFW_KEY_ENTER:
				case GLFW_KEY_KP_ENTER:
					{
						// TODO: Do something on enter, like insert the selected autocompletion into parent

						ModifyParent()->RemoveWidget(this);
						g_InputManager->RequestTypingPointer(ModifyParent()->ModifyGestureRecognizer());
					}
					break;
				case GLFW_KEY_TAB:
					{
					}
					break;
				case GLFW_KEY_LEFT:
				case GLFW_KEY_RIGHT:
					{
						g_InputManager->RequestTypingPointer(ModifyParent()->ModifyGestureRecognizer());
						ModifyParent()->RemoveWidget(this);		// TODO: Maybe this is wrong to do, verify
						HandledEvent = false;		// TODO: Make this work
					}
					break;
				case GLFW_KEY_UP:
					{
						if (m_SelectedEntry > 0)
							--m_SelectedEntry;
					}
					break;
				case GLFW_KEY_DOWN:
					{
						if (m_SelectedEntry < m_Entries.size() - 1)
							++m_SelectedEntry;
					}
					break;
				default:
					HandledEvent = false;
					break;
				}

				if (HandledEvent)
				{
					InputEvent.m_Handled = true;
				}
			}
		}
		else if (Pointer::VirtualCategory::POINTING == InputEvent.m_Pointer->GetVirtualCategory())
		{
			if (Pressed)
			{
				{
					InputEvent.m_Handled = true;
				}

				switch (ButtonId)
				{
				case 0:
					{
					}
					break;
				default:
					break;
				}
			}
		}
	}

	// TODO: Re-enable this but debug the crashing it causes, etc.
	if (   InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::AXIS_EVENT)
		|| InputEvent.m_EventTypes.end() != InputEvent.m_EventTypes.find(InputEvent::EventType::CANVAS_MOVED_TEST))
	{
		if (Pointer::VirtualCategory::POINTING == InputEvent.m_Pointer->GetVirtualCategory())
		{
			if (true == InputEvent.m_Pointer->GetPointerState().GetButtonState(0))
			{
				Vector2n GlobalPosition(InputEvent.m_Pointer->GetPointerState().GetAxisState(0).GetPosition(), InputEvent.m_Pointer->GetPointerState().GetAxisState(1).GetPosition());
				Vector2n LocalPosition = GlobalToLocal(GlobalPosition);

				if (LocalPosition.Y() < 0)
					m_SelectedEntry = 0;
				else
					m_SelectedEntry = LocalPosition.Y() / lineHeight;
				if (m_SelectedEntry > m_Entries.size() - 1)
					m_SelectedEntry = m_Entries.size() - 1;

			}
		}
	}
}

template <typename T> void ContextMenuWidget<T>::UpdateDimensions()
{
	Vector2n Dimensions;

	for (auto & Entry : m_Entries)
	{
		Dimensions.X() = std::max<sint32>(Dimensions.X(), Concept::GetDimensions(Entry).X());
		Dimensions.Y() += Concept::GetDimensions(Entry).Y();
	}

	Vector2n MinDimensions(3 * charWidth, lineHeight);
	Dimensions.X() = std::max<sint32>(Dimensions.X(), MinDimensions.X());
	Dimensions.Y() = std::max<sint32>(Dimensions.Y(), MinDimensions.Y());

	SetDimensions(Dimensions);
}
