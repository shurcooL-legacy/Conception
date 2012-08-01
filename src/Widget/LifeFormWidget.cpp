#include "../Main.h"

LifeFormWidget::LifeFormWidget(Vector2n Position)
	: Widget(Position, Vector2n(100, 100)),
//	  m_GestureRecognizerTEST(),
	  m_Color(0),
	  m_PositionD(Position.X(), Position.Y()), 
	  m_VelocityD(0, 0)
{
	ModifyGestureRecognizer().m_RecognizeTap = true;
}

LifeFormWidget::~LifeFormWidget()
{
}

void LifeFormWidget::Render()
{
	const Color Colors[] =
	{
		Color(0 / 255.0, 140 / 255.0, 0 / 255.0),
		Color(0 / 255.0, 98 / 255.0, 140 / 255.0),
		Color(194 / 255.0, 74 / 255.0, 0 / 255.0),
		Color(89 / 255.0, 0 / 255.0, 140 / 255.0),
		Color(191 / 255.0, 150 / 255.0, 0 / 255.0),
		Color(140 / 255.0, 0 / 255.0, 0 / 255.0)
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

	DrawBox(m_Position, m_Dimensions, BackgroundColor, BorderColor);
}

void LifeFormWidget::ProcessTap()
{
	++m_Color;
	if (m_Color >= 6)
		m_Color = 0;

	g_InputManager->RequestTypingPointer(ModifyGestureRecognizer());
}

void LifeFormWidget::ProcessTimePassed(const double TimePassed)
{
	m_VelocityD = Vector2d(8, 5);

	for (auto & Connected : GetGestureRecognizer().GetConnected())
	{
		if (Pointer::VirtualCategory::POINTING == Connected->GetVirtualCategory())
		{
			const PointerState & PointerState = Connected->GetPointerState();
			
			const double SpeedMultiplier = 250;
			
			Vector2n GlobalPosition(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition());
			Vector2n LocalPosition = GlobalToLocal(GlobalPosition);
			
			if (   true//PointerState.GetButtonState(0)
				&& LocalPosition.X() >= 0
				&& LocalPosition.Y() >= 0
				&& LocalPosition.X() <= m_Dimensions.X()
				&& LocalPosition.Y() <= m_Dimensions.Y())
			{
				auto EscapeVector = LocalPosition - m_Dimensions / 2;
				auto EscapeAngle = std::atan2(EscapeVector.X(), EscapeVector.Y());
				auto EscapeDirection = Vector2d(std::sin(EscapeAngle), std::cos(EscapeAngle)) * -1;
				
				m_VelocityD = EscapeDirection * SpeedMultiplier;
			}
		}
	}

	// Simulation
	{
		m_PositionD = m_PositionD + m_VelocityD * TimePassed;

		m_Position.X() = m_PositionD.X();
		m_Position.Y() = m_PositionD.Y();
	}
}
