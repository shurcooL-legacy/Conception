#include "../Main.h"

ToggleWidget::ToggleWidget(Vector2n Position, Vector2n Dimensions, std::function<void(bool)> OnChange, bool State)
	: ButtonWidget(Position, Dimensions, [&]() { m_State = !m_State; if (nullptr != m_OnChange) m_OnChange(m_State); }),
	  m_State(State),
	  m_OnChange(OnChange)
{
	// TODO: Disabled because it causes crashing by doing stuff that isn't created yet... delay this to once everything is constructed or something?
	/*if (nullptr != m_OnChange)
		m_OnChange(m_State);*/
}

ToggleWidget::~ToggleWidget()
{
}

void ToggleWidget::Render()
{
	// TODO: Refactor this out
	if (!m_Visible)
		return;

	Color BackgroundColor = m_State ? Color(0.9, 1, 0.9) : Color(1.0, 0.9, 0.9);
	Color BorderColor(0.5, 0.5, 0.5);

	if (CheckHover() && CheckActive())
	{
		BackgroundColor[0] = 0.75;
		BackgroundColor[1] = 0.75;
		BackgroundColor[2] = 0.75;
		BorderColor[0] = 0.898;
		BorderColor[1] = 0.765;
		BorderColor[2] = 0.396;
	}
	//else if ((CheckHover() && !CheckAnyActive()) || (!CheckHover() && CheckActive()))
	else if (CheckHover() ^ CheckActive())
	{
		BorderColor[0] = 0.898;
		BorderColor[1] = 0.765;
		BorderColor[2] = 0.396;
	}
	else
	{
	}

	if (Vector2n(40, 16) == GetDimensions())
	{
		DrawInnerRoundedBox(GetPosition(), Vector2n(40, 16), 8, BackgroundColor, BorderColor);

		glColor3d(0, 0, 0);
		if (m_State)
			OglUtilsPrint(GetPosition().X() + 12+3, GetPosition().Y() + 0.5, 0, PrintAlignment::CENTER, "1");
		else
			OglUtilsPrint(GetPosition().X() + 28-3, GetPosition().Y() + 0.5, 0, PrintAlignment::CENTER, "0");

		DrawCircle(GetPosition() + Vector2n(8 + 24 * m_State, 8), Vector2n(14, 14), CheckHover() ? BorderColor : Color(static_cast<uint8>(240), 240, 240), BorderColor * 0.7);
	}
	else
	{
		// TODO: Unfinished... this looks bad, hard to differentiate
		DrawBox(GetPosition(), GetDimensions(), BackgroundColor, BorderColor);

		glColor3d(0, 0, 0);
		if (m_State)
			OglUtilsPrint(GetPosition().X() + GetDimensions().X() / 2, GetPosition().Y() + (GetDimensions().Y() - lineHeight) / 2 + 0.5, 0, PrintAlignment::CENTER, "+");
		else
			OglUtilsPrint(GetPosition().X() + GetDimensions().X() / 2, GetPosition().Y() + (GetDimensions().Y() - lineHeight) / 2 + 0.5, 0, PrintAlignment::CENTER, "-");
	}
}

bool ToggleWidget::GetState() const
{
	return m_State;
}

void ToggleWidget::UpdateHACK()
{
	m_OnChange(m_State);
}
