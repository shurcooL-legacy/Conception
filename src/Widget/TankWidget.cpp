#include "../Main.h"

TankWidget::TankWidget(Vector2n Position)
	: Widget(Position, Vector2n(30, 30), {}),
	  m_TargetPosition(Position),
	  m_PositionD(Position.X(), Position.Y())
{
}

TankWidget::~TankWidget()
{
}

void TankWidget::Render()
{
	// TODO: Refactor this out
	if (!m_Visible)
		return;

	Color BackgroundColor(0.99, 0.99, 0.99);
	Color BorderColor(0.5, 0.5, 0.5);

	if (CheckActive())
	{
		BackgroundColor[0] = 0.75;
		BackgroundColor[1] = 0.75;
		BackgroundColor[2] = 0.75;
		BorderColor[0] = 0.898;
		BorderColor[1] = 0.765;
		BorderColor[2] = 0.396;
	}
	//else if ((CheckHover() && !CheckAnyActive()) || (!CheckHover() && CheckActive()))
	else if (CheckHover() && !CheckActive())
	{
		BackgroundColor[0] = 1.0;
		BackgroundColor[1] = 1.0;
		BackgroundColor[2] = 1.0;
		BorderColor[0] = 0.898;
		BorderColor[1] = 0.765;
		BorderColor[2] = 0.396;
	}
	else
	{
	}

	auto Center = GetPosition() + GetDimensions() / 2;

	if (CheckActive())
	{
		for (auto & Pointer : GetGestureRecognizer().GetConnected())
		{
			if (Pointer::VirtualCategory::POINTING == Pointer->GetVirtualCategory())
			{
				const auto & PointerState = Pointer->GetPointerState();
				Vector2n GlobalPosition(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition());
				Vector2n LocalPosition = GlobalToLocal(GlobalPosition);

				// TODO: Replace this with a resolution independent quad-line
				glColor3dv(BorderColor.GetComponents());
				glBegin(GL_LINES);
					glVertex2i(Center.X(), Center.Y());
					glVertex2i(GetPosition().X() + LocalPosition.X(), GetPosition().Y() + LocalPosition.Y());
				glEnd();
			}
		}
	}

	DrawBox(GetPosition(), GetDimensions(), BackgroundColor, BorderColor);
	DrawCircle(Center, Vector2n(26, 26), BackgroundColor, BorderColor);
	DrawCircle(Center, Vector2n(6, 6), BorderColor, BorderColor);
}

void TankWidget::ProcessEvent(InputEvent & InputEvent)
{
	if (IsPointerButtonEvent<Pointer::VirtualCategory::POINTING, 0, false>(InputEvent))
	{
		const auto & PointerState = InputEvent.m_Pointer->GetPointerState();
		Vector2n GlobalPosition(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition());
		Vector2n ParentPosition = GlobalToParent(GlobalPosition);

		m_TargetPosition = ParentPosition - GetDimensions() / 2;

		InputEvent.m_Handled = true;
	}

	// HACK: Block all events other than vertical axis scroll events
	if (!(   InputEvent.HasType(InputEvent::EventType::AXIS_EVENT)
		  && 2 == InputEvent.m_InputId))
	{
		InputEvent.m_Handled = true;
	}
}

void TankWidget::ProcessTimePassed(const double TimePassed)
{
	Vector2d m_VelocityD = Vector2d(m_TargetPosition.X() - GetPosition().X(), m_TargetPosition.Y() - GetPosition().Y());
	if (m_VelocityD.LengthSquared() > 0)
		m_VelocityD = m_VelocityD / (sqrt(m_VelocityD.LengthSquared()) / 60);
	else
		m_VelocityD = Vector2d::ZERO;

	// Simulation
	{
		m_PositionD = m_PositionD + m_VelocityD * TimePassed;

		ModifyPosition().X() = m_PositionD.X();
		ModifyPosition().Y() = m_PositionD.Y();
	}
}
