template <typename T> ListWidget<T>::ListWidget(Vector2n Position, std::vector<T> & List)
	: CompositeWidget(Position, {
		std::shared_ptr<Widget>(new ButtonWidget(Vector2n(-1, -18), Vector2n(lineHeight, lineHeight), [&]() {
			// TEST: This is specific stuff for quick testing
			if (!m_List.empty())
			{
				m_List.pop_back();
			}
		} )) }),
	  m_List(List)
{
	ModifyGestureRecognizer().m_RecognizeTap = true;
	ModifyGestureRecognizer().m_RecognizeManipulationTranslate = true;

	UpdateDimensions();
}

template <typename T> ListWidget<T>::~ListWidget()
{
}

template <typename T> void ListWidget<T>::Insert(T & Entry)
{
	m_List.push_back(Entry);
}

template <typename T> void ListWidget<T>::Render()
{
	Color BackgroundColor(1, 1, 1);
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

		if (m_List.empty())
		{
			BackgroundColor[0] = 234 / 255.0;
			BackgroundColor[1] = 233 / 255.0;
			BackgroundColor[2] = 190 / 255.0;
		}

		DrawAroundBox(GetPosition(), GetDimensions(), BackgroundColor, BorderColor);

		// TODO: This part is not general
		std::string Description[2] = { "#include <", ">" };
		glColor3d(0, 0, 0); OglUtilsPrint(GetPosition().X(), GetPosition().Y(), 0, RIGHT, Description[0].c_str());
		glColor3d(0, 0, 0); OglUtilsPrint(GetPosition().X() + GetDimensions().X(), GetPosition().Y(), 0, LEFT, Description[1].c_str());

		OpenGLStream OpenGLStream(GetPosition());
		for (auto & Entry : m_List)
		{
			OpenGLStream << Entry << endl;
		}
	}

	CompositeWidget::Render();
}

template <typename T> void ListWidget<T>::ProcessTap()
{
	m_TapAction();
}

template <typename T> void ListWidget<T>::UpdateDimensions()
{
	Vector2n MinDimensions(3 * charWidth, lineHeight);

	Vector2n MaxDimensions(MinDimensions);
	for (auto & Entry : m_List)
	{
		MaxDimensions.X() = std::max<sint32>(MaxDimensions.X(), Concept::GetDimensions(Entry).X());
	}
	MaxDimensions.Y() = std::max<sint32>(MaxDimensions.Y(), static_cast<sint32>(m_List.size() + 1) * lineHeight);

	Vector2n Dimensions = MaxDimensions;

	SetDimensions(Dimensions);
}

template <typename T> void ListWidget<T>::ProcessManipulationStarted(const PointerState & PointerState)
{
	if (ModifyGestureRecognizer().GetConnected().end() == ModifyGestureRecognizer().GetConnected().find(g_InputManager->m_TypingPointer.get()))
	{
		auto ParentLocalPosition = GlobalToParent(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));
		
		ModifyGestureRecognizer().m_ManipulationOffset = GetPosition() - ParentLocalPosition;
	}
}

template <typename T> void ListWidget<T>::ProcessManipulationUpdated(const PointerState & PointerState)
{
	if (ModifyGestureRecognizer().GetConnected().end() == ModifyGestureRecognizer().GetConnected().find(g_InputManager->m_TypingPointer.get()))
	{
		auto ParentLocalPosition = GlobalToParent(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));
		
		ModifyPosition() = GetGestureRecognizer().m_ManipulationOffset + ParentLocalPosition;
	}
}

template <typename T> void ListWidget<T>::ProcessManipulationCompleted(const PointerState & PointerState)
{
}
