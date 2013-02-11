template <typename T> ListWidget<T>::ListWidget(Vector2n Position, std::vector<T> & List, TypingModule & TypingModule)
#if DECISION_LIST_WIDGET_IS_COMPOSITE
	: CompositeWidget(Position, {
		std::shared_ptr<Widget>(new ButtonWidget(Vector2n(-1, -17), Vector2n(lineHeight, lineHeight), [&]() {
			// TEST: This is specific stuff for quick testing
			if (!m_Entries.empty())
			{
				m_Entries.pop_back();
			}
		} )) }, {}),
#else
	: Widget(Position, Vector2n::ZERO, {}),
#endif
	  m_Entries(List),
	  m_TypingModule(TypingModule)
{
	ModifyGestureRecognizer().m_RecognizeTap = true;

	UpdateDimensions();
}

template <typename T> ListWidget<T>::~ListWidget()
{
}

template <typename T> void ListWidget<T>::Insert(T & Entry)
{
	m_Entries.push_back(Entry);
}

template <typename T> void ListWidget<T>::Render()
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

		// TEST
		auto Spot = m_Entries.end();
		if (!m_TypingModule.GetString().empty())
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
		for (auto Entry = m_Entries.begin(); m_Entries.end() != Entry; ++Entry)
		{
			if (Entry == Spot)
				OpenGLStream << endl;

			OpenGLStream << *Entry << endl;
		}
	}

#if DECISION_LIST_WIDGET_IS_COMPOSITE
	CompositeWidget::Render();
#endif
}

template <typename T> void ListWidget<T>::ProcessTap(const InputEvent & InputEvent, Vector2n Position)
{
	if (nullptr != m_TapAction)
	{
		m_TapAction(GlobalToLocal(Position), m_Entries);
	}
}

template <typename T> void ListWidget<T>::UpdateDimensions()
{
	Vector2n Dimensions;

	for (auto & Entry : m_Entries)
	{
		Dimensions.X() = std::max<sint32>(Dimensions.X(), Concept::GetDimensions(Entry).X());
		Dimensions.Y() += Concept::GetDimensions(Entry).Y();
	}

	// TEST
	if (!m_TypingModule.GetString().empty())
	{
		for (auto & Pointer : GetGestureRecognizer().GetConnected())
		{
			if (Pointer::VirtualCategory::POINTING == Pointer->GetVirtualCategory())
			{
				Dimensions.X() = std::max<sint32>(Dimensions.X(), static_cast<sint32>(m_TypingModule.GetString().length() * charWidth));
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
