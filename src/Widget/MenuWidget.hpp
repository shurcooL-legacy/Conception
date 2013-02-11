template <typename T> MenuWidget<T>::MenuWidget(Vector2n Position, std::vector<T> & Entries)
	: Widget(Position, Vector2n::ZERO, {}),
	  m_Entries(Entries)
{
	ModifyGestureRecognizer().m_RecognizeTap = true;

	UpdateDimensions();
}

template <typename T> MenuWidget<T>::~MenuWidget()
{
}

template <typename T> const T * MenuWidget<T>::GetSelectedEntry() const
{
	if (!m_Entries.empty())
		return &m_Entries[m_SelectedEntryId];
	else
		return nullptr;
}

/*template <typename T> void MenuWidget<T>::SetSelectedEntryId(decltype(m_SelectedEntryId) SelectedEntryId)
{
	m_SelectedEntryId = SelectedEntryId;
}*/

template <typename T> void MenuWidget<T>::SetSelectedEntryId(Vector2n LocalPosition)
{
	if (LocalPosition.Y() < 0)
		m_SelectedEntryId = 0;
	else
		m_SelectedEntryId = LocalPosition.Y() / lineHeight;
	if (m_SelectedEntryId > m_Entries.size() - 1)
		m_SelectedEntryId = m_Entries.size() - 1;
}

template <typename T> void MenuWidget<T>::Render()
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
	// HACK, TODO: Make this a single DRY const
	if (HasTypingFocus())
	{
		BorderColor[0] = 0.898;
		BorderColor[1] = 0.765;
		BorderColor[2] = 0.396;
	}

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
			if (Entry - m_Entries.begin() == m_SelectedEntryId)
			{
				DrawBox(GetPosition() + Vector2n(0, static_cast<sint32>((Entry - m_Entries.begin()) * lineHeight)), Vector2n(GetDimensions().X(), lineHeight), m_SelectedColor, m_SelectedColor);
			}

			OpenGLStream << *Entry << endl;
		}
	}
}

template <typename T> void MenuWidget<T>::ProcessTap(const InputEvent & InputEvent, Vector2n Position)
{
	if (nullptr != m_TapAction)
	{
		m_TapAction(GlobalToLocal(Position), m_Entries);
	}
}

template <typename T> void MenuWidget<T>::ProcessDoubleTap(const InputEvent & InputEvent, Vector2n Position)
{
	if (nullptr != m_DoubleTapAction)
	{
		m_DoubleTapAction(GlobalToLocal(Position), m_Entries);
	}
}

template <typename T> void MenuWidget<T>::ProcessEvent(InputEvent & InputEvent)
{
	if (InputEvent.HasType(InputEvent::EventType::BUTTON_EVENT))
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
				case GLFW_KEY_UP:
					{
						if (m_SelectedEntryId > 0)
							--m_SelectedEntryId;
					}
					break;
				case GLFW_KEY_DOWN:
					{
						if (m_SelectedEntryId < m_Entries.size() - 1)
							++m_SelectedEntryId;
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
	if (   InputEvent.HasType(InputEvent::EventType::AXIS_EVENT)
		|| InputEvent.HasType(InputEvent::EventType::CANVAS_MOVED_TEST))
	{
		if (Pointer::VirtualCategory::POINTING == InputEvent.m_Pointer->GetVirtualCategory())
		{
			if (true == InputEvent.m_Pointer->GetPointerState().GetButtonState(0))
			{
				Vector2n GlobalPosition(InputEvent.m_Pointer->GetPointerState().GetAxisState(0).GetPosition(), InputEvent.m_Pointer->GetPointerState().GetAxisState(1).GetPosition());
				Vector2n LocalPosition = GlobalToLocal(GlobalPosition);

				SetSelectedEntryId(LocalPosition);
			}
		}
	}
}

template <typename T> void MenuWidget<T>::UpdateDimensions()
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
