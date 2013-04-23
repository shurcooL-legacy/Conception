template <typename T> MenuWidget<T>::MenuWidget(Vector2n Position, std::vector<T> & Entries, TypingModule * TypingModule)
  : Widget(Position, Vector2n::ZERO, {}),
	m_Entries(Entries),
	m_TypingModule(TypingModule)
{
	ModifyGestureRecognizer().m_RecognizeTap = true;

	UpdateDimensions();
}

template <typename T> MenuWidget<T>::~MenuWidget()
{
}

template <typename T> const T * MenuWidget<T>::GetSelectedEntry() const
{
	if (   m_SelectedEntryId >= 0
		&& m_SelectedEntryId < m_Entries.size())
		return &m_Entries[m_SelectedEntryId];
	else
		return nullptr;
}

template <typename T> void MenuWidget<T>::SetSelectedEntryId(Vector2n LocalPosition)
{
	decltype(m_SelectedEntryId) NewSelectedEntryId;

	if (LocalPosition.Y() < 0)
		NewSelectedEntryId = 0;
	else
		NewSelectedEntryId = LocalPosition.Y() / lineHeight;
	if (NewSelectedEntryId > m_Entries.size() - 1)
		NewSelectedEntryId = m_Entries.size() - 1;

	SetSelectedEntryId(NewSelectedEntryId);
}

template <typename T> void MenuWidget<T>::SetSelectedEntryId(decltype(m_SelectedEntryId) SelectedEntryId)
{
	if (m_SelectedEntryId != SelectedEntryId)
	{
		m_SelectedEntryId = SelectedEntryId;

		if (nullptr != m_OnChange) {
			m_OnChange();
		}
	}
}

template <typename T> void MenuWidget<T>::UpdateEntries(const std::vector<T> & Entries)
{
	if (m_Entries != Entries)
	{
		bool SelectionChangedToNone = false;

		// Figure out what the m_SelectedEntryId should be for the new Entries
		if (m_SelectedEntryId < m_Entries.size())
		{
			auto SelectedEntry = m_Entries[m_SelectedEntryId];

			SelectionChangedToNone = true;		// Assume true until for loop...
			for (auto it0 = Entries.begin(); Entries.end() != it0; ++it0)
			{
				if (SelectedEntry == *it0)
				{
					m_SelectedEntryId = it0 - Entries.begin();		// The new entries contain the same selected entry, so just update m_SelectedEntryId and do nothing else
					SelectionChangedToNone = false;					// Unless got here
					break;
				}
			}
		}

		m_Entries = Entries;

		if (SelectionChangedToNone)		// If not found (i.e. changed from something selected to nothing selected), trigger an OnChange event
			SetSelectedEntryId(-1);
	}
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

		// TEST
		auto Spot = m_Entries.end();
		if (   nullptr != m_TypingModule
			&& !m_TypingModule->GetString().empty())
		{
			for (auto & Pointer : GetGestureRecognizer().GetConnected())
			{
				if (Pointer::VirtualCategory::POINTING == Pointer->GetVirtualCategory())
				{
					Vector2n GlobalPosition(Pointer->GetPointerState().GetAxisState(0).GetPosition(), Pointer->GetPointerState().GetAxisState(1).GetPosition());
					Vector2n LocalPosition(GlobalToLocal(GlobalPosition));

					Spot = m_Entries.begin() + (LocalPosition.Y() / lineHeight);
				}
			}
		}

		OpenGLStream OpenGLStream(GetPosition());
		//for (auto & Entry : m_Entries)
		for (auto Entry = m_Entries.begin(); m_Entries.end() != Entry; ++Entry)
		{
			if (Entry == Spot)
				OpenGLStream << endl;

			if (Entry - m_Entries.begin() == m_SelectedEntryId)
			{
				if (HasTypingFocus())
					DrawBox(GetPosition() + Vector2n(0, static_cast<sint32>((Entry - m_Entries.begin() + (Entry >= Spot)) * lineHeight)), Vector2n(GetDimensions().X(), lineHeight), m_SelectedColor, m_SelectedColor);
				else
					DrawBox(GetPosition() + Vector2n(0, static_cast<sint32>((Entry - m_Entries.begin() + (Entry >= Spot)) * lineHeight)), Vector2n(GetDimensions().X(), lineHeight), m_UnfocusedSelectedColor, m_UnfocusedSelectedColor);
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
				const auto ControlActive = (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LCTRL)
											|| InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RCTRL));
				const auto ShiftActive = (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LSHIFT)
										  || InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RSHIFT));
				const auto SuperActive = (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LSUPER)
										  || InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RSUPER));
				const auto AltActive = (   InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_LALT)
										|| InputEvent.m_Pointer->GetPointerState().GetButtonState(GLFW_KEY_RALT));

				bool HandledEvent = true;		// Assume true at first

				switch (ButtonId)
				{
				case GLFW_KEY_UP:
					{
						if (!SuperActive) {
							if (m_SelectedEntryId > 0)
								SetSelectedEntryId(m_SelectedEntryId - 1);
						} else {
							SetSelectedEntryId(0);
						}
					}
					break;
				case GLFW_KEY_DOWN:
					{
						if (!SuperActive) {
							if (m_SelectedEntryId < m_Entries.size() - 1)
								SetSelectedEntryId(m_SelectedEntryId + 1);
						} else {
							SetSelectedEntryId(m_Entries.size() - 1);
						}
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
	}

	if (HasTypingFocus())
	{
		if (   InputEvent.HasType(InputEvent::EventType::AXIS_EVENT)
			|| InputEvent.HasType(InputEvent::EventType::CANVAS_MOVED_TEST))
		{
			if (Pointer::VirtualCategory::POINTING == InputEvent.m_Pointer->GetVirtualCategory())
			{
				if (true == InputEvent.m_Pointer->GetPointerState().GetButtonState(0))
				{
					if (!(   nullptr != m_TypingModule
						  && !m_TypingModule->GetString().empty()))
					{
						Vector2n GlobalPosition(InputEvent.m_Pointer->GetPointerState().GetAxisState(0).GetPosition(), InputEvent.m_Pointer->GetPointerState().GetAxisState(1).GetPosition());
						Vector2n LocalPosition = GlobalToLocal(GlobalPosition);

						SetSelectedEntryId(LocalPosition);
					}
				}
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

	// TEST
	if (   nullptr != m_TypingModule
		&& !m_TypingModule->GetString().empty())
	{
		for (auto & Pointer : GetGestureRecognizer().GetConnected())
		{
			if (Pointer::VirtualCategory::POINTING == Pointer->GetVirtualCategory())
			{
				Dimensions.X() = std::max<sint32>(Dimensions.X(), static_cast<sint32>(m_TypingModule->GetString().length() * charWidth));
				Dimensions.Y() += 1 * lineHeight;
				break;
			}
		}
	}

	Vector2n MinDimensions(3 * charWidth, lineHeight);
	Dimensions.X() = std::max<sint32>(Dimensions.X(), MinDimensions.X());
	Dimensions.Y() = std::max<sint32>(Dimensions.Y(), MinDimensions.Y());

	SetDimensions(Dimensions);
}
