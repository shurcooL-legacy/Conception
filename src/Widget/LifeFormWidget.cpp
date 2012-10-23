#include "../Main.h"

LifeFormWidget::LifeFormWidget(Vector2n Position)
	: Widget(Position, Vector2n(100, 100), {}),
//	  m_GestureRecognizerTEST(),
	  m_Color(0),
	  m_PositionD(Position.X(), Position.Y()), 
	  m_VelocityD(0, 0),
	  m_CurrentState()
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
		Color(static_cast<uint8>(0), 140, 0),
		Color(static_cast<uint8>(0), 98, 140),
		Color(static_cast<uint8>(194), 74, 0),
		Color(static_cast<uint8>(89), 0, 140),
		Color(static_cast<uint8>(191), 150, 0),
		Color(static_cast<uint8>(140), 0, 0)
	};
	
	Color BackgroundColor(Colors[m_Color]);
	DrawCircle(GetPosition(), GetDimensions(), BackgroundColor);

	// Render properties
	OpenGLStream OpenGLStream(GetPosition() + Vector2n(GetDimensions().X() / 2 + 5, GetDimensions().X() / -2));
	std::stringstream ss;
	ss << "Health: " << m_CurrentState.Health << '\n';
	ss << "Pain: " << m_CurrentState.Pain << '\n';
	ss << "Energy: " << m_CurrentState.Energy;
	OpenGLStream << ss.str();
}

bool LifeFormWidget::IsHit(const Vector2n ParentPosition) const
{
	bool Hit = (ParentPosition - GetPosition()).LengthSquared() < (GetDimensions().X() * GetDimensions().X() / 4);

	return Hit;
}

void LifeFormWidget::ProcessTap(InputEvent & InputEvent, Vector2n Position)
{
	++m_Color;
	if (m_Color >= 6)
		m_Color = 0;

	g_InputManager->RequestTypingPointer(ModifyGestureRecognizer());
}

LifeFormWidget::Output LifeFormWidget::GenerateOutput()
{
	Output Output;

	Output.Action = std::rand() % 9 - 1;

	return Output;
}

void LifeFormWidget::ProcessTimePassed(const double TimePassed)
{
#if 1
	// Instinct model
	{
		m_VelocityD = Vector2d(8, 5);

		for (auto & Pointer : GetGestureRecognizer().GetConnected())
		{
			if (   Pointer::VirtualCategory::POINTING == Pointer->GetVirtualCategory()
#if !DECISION_POINTER_MAPPING_CONTAINS_SINGLE_TOPMOST_WIDGET
				&& &GetGestureRecognizer() == Pointer->GetPointerMapping().GetHoverer()
#endif
				)
			{
				const PointerState & PointerState = Pointer->GetPointerState();
				
				const double SpeedMultiplier = 250;
				
				Vector2n GlobalPosition(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition());
				Vector2n ParentPosition = GlobalToParent(GlobalPosition);
				Vector2n LocalPosition = ParentToLocal(ParentPosition);

				if (   true//PointerState.GetButtonState(0)
					&& IsHit(ParentPosition))
				{
					auto EscapeVector = LocalPosition;
					auto EscapeAngle = std::atan2(EscapeVector.X(), EscapeVector.Y());
					auto EscapeDirection = Vector2d(std::sin(EscapeAngle), std::cos(EscapeAngle)) * -1;
					
					m_VelocityD = EscapeDirection * SpeedMultiplier;
				}
			}
		}
	}
#else
	// Thinking model
	{
		const auto QuarterPi = std::atan(1);

		auto Output = GenerateOutput();

		m_VelocityD = Vector2d(0, 0);

		if (   Output.Action >= 0
			&& Output.Action <  8)
		{
			m_VelocityD = Vector2d(std::cos(QuarterPi * Output.Action), std::sin(QuarterPi * Output.Action)) * 10;
		}
	}
#endif

	// Simulation
	{
		m_PositionD = m_PositionD + m_VelocityD * TimePassed;

		ModifyPosition().X() = m_PositionD.X();
		ModifyPosition().Y() = m_PositionD.Y();
	}
}
