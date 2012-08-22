#include "../Main.h"

MultitouchTestBoxWidget::MultitouchTestBoxWidget(Vector2n Position)
	: Widget(Position, Vector2n(200, 200)),
//	  m_GestureRecognizerTEST(),
	  m_Color(rand() % 6)		// HACK
{
//	m_GestureRecognizerTEST.m_Widget = this;
//	m_GestureRecognizerTEST.m_OnTap = &MultitouchTestBoxWidget::ProcessTap;
	ModifyGestureRecognizer().m_RecognizeTap = true;
	ModifyGestureRecognizer().m_RecognizeManipulationTranslate = true;
}

MultitouchTestBoxWidget::~MultitouchTestBoxWidget()
{
}

void MultitouchTestBoxWidget::Render()
{
	const Color Colors[] =
	{
		Color(static_cast<uint8>(0), 140, 0),
		Color(static_cast<uint8>(0), 98, 140),
		Color(static_cast<uint8>(194), 74, 0),
		Color(static_cast<uint8>(89), 0, 140),
		Color(static_cast<uint8>(191), 150, 0),
		Color(static_cast<uint8>(140), 0, 0)
	};

	Color BackgroundColor(Colors[m_Color]);
	//Color BorderColor(0.0, 0.0, 0.0);
	Color BorderColor(BackgroundColor);

	if (CheckHover())
	{
		BorderColor[0] = 0.898;
		BorderColor[1] = 0.765;
		BorderColor[2] = 0.396;
	}

	DrawBox(GetPosition(), GetDimensions(), BackgroundColor, BorderColor);
}

void MultitouchTestBoxWidget::ProcessTap(InputEvent & InputEvent, Vector2n Position)
{
	++m_Color;
	if (m_Color >= 6)
		m_Color = 0;

	g_InputManager->RequestTypingPointer(ModifyGestureRecognizer());
}

#if 0
void MultitouchTestBoxWidget::ProcessButton(Pointer * Pointer, Input::InputId ButtonId, bool Pressed)
{
	if (0 == ButtonId)
	{
		if (Pressed)
		{
			//Pointer->GetWidgetMapping().RequestActivation(this);
		}
		else
		{
			//Pointer->GetWidgetMapping().RequestDeactivation(this);
		}
	}
}

void MultitouchTestBoxWidget::ProcessSlider(Pointer * Pointer, Input::InputId SliderId, double MovedAmount)
{
	// TODO: Make it based on this widget being active/having captured the pointer, rather than pointer being active (i.e. not exclusive)
	//if (Pointer->IsActive())
	//if (IsActiveExternally())
	//if (this == Pointer->GetWidgetMapping().GetActiveWidget())
	{
		if (0 == SliderId)
		{
			m_Position.X() += static_cast<sint32>(MovedAmount);
		}
		else if (1 == SliderId)
		{
			m_Position.Y() += static_cast<sint32>(MovedAmount);
		}
	}

	/*if (2 == SliderId)
	{
		/ *double A[2] = { (GetAxisState(0).GetPosition() - 0.5 * GetAxisState(0).GetLength()),
						(GetAxisState(1).GetPosition() - 0.5 * GetAxisState(1).GetLength()) };* /
		double A[2] = { 0, 0 };
		MoveView(static_cast<uint8>(SliderId), MovedAmount, A);
	}*/
}
#endif

/*void MultitouchTestBoxWidget::ProcessDrag(Vector2d DragAmount)
{
	//m_Position.X() += static_cast<sint32>(DragAmount.X());
	//m_Position.Y() += static_cast<sint32>(DragAmount.Y());
}*/

void MultitouchTestBoxWidget::ProcessManipulationStarted(const PointerState & PointerState)
{
	//printf("MultitouchTestBoxWidget::ProcessManipulationStarted()\n");

	/*Vector2d PositionDouble = GetParent()->GlobalToCanvas(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));
	Vector2n PositionInt(std::lround(PositionDouble.X()), std::lround(PositionDouble.Y()));		// TODO: Loss of accuracy? Fix it if needed.*/
	auto ParentLocalPosition = GlobalToParent(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));

	ModifyGestureRecognizer().m_ManipulationOffset = GetPosition() - ParentLocalPosition;
}

void MultitouchTestBoxWidget::ProcessManipulationUpdated(const PointerState & PointerState)
{
	//printf("MultitouchTestBoxWidget::ProcessManipulationUpdated()\n");

	/*Vector2d PositionDouble = GetParent()->GlobalToCanvas(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));
	Vector2n PositionInt(std::lround(PositionDouble.X()), std::lround(PositionDouble.Y()));		// TODO: Loss of accuracy? Fix it if needed.*/
	auto ParentLocalPosition = GlobalToParent(Vector2n(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition()));

	ModifyPosition() = GetGestureRecognizer().m_ManipulationOffset + ParentLocalPosition;
}

void MultitouchTestBoxWidget::ProcessManipulationCompleted(const PointerState & PointerState)
{
	//printf("MultitouchTestBoxWidget::ProcessManipulationCompleted()\n");
}

void MultitouchTestBoxWidget::ProcessTimePassed(const double TimePassed)
{
	for (auto & Pointer : GetGestureRecognizer().GetConnected())
	{
		if (Pointer::VirtualCategory::TYPING == Pointer->GetVirtualCategory())
		{
			const PointerState & PointerState = Pointer->GetPointerState();

			const double SpeedMultiplier = 250;

			if (PointerState.GetButtonState(GLFW_KEY_LEFT) && !PointerState.GetButtonState(GLFW_KEY_RIGHT))
			{
				ModifyPosition().X() += -SpeedMultiplier * TimePassed;
			}
			else if (PointerState.GetButtonState(GLFW_KEY_RIGHT) && !PointerState.GetButtonState(GLFW_KEY_LEFT))
			{
				ModifyPosition().X() += SpeedMultiplier * TimePassed;
			}

			if (PointerState.GetButtonState(GLFW_KEY_UP) && !PointerState.GetButtonState(GLFW_KEY_DOWN))
			{
				ModifyPosition().Y() += -SpeedMultiplier * TimePassed;
			}
			else if (PointerState.GetButtonState(GLFW_KEY_DOWN) && !PointerState.GetButtonState(GLFW_KEY_UP))
			{
				ModifyPosition().Y() += SpeedMultiplier * TimePassed;
			}
		}
	}
}
