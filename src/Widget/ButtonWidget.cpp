#include "../Main.h"

ButtonWidget::ButtonWidget(Vector2n Position, Vector2n Dimensions, std::function<void()> Action)
	: Widget(Position, Dimensions),
	  m_Action(Action)
{
	ModifyGestureRecognizer().m_RecognizeTap = true;
}

ButtonWidget::~ButtonWidget()
{
}

void ButtonWidget::Render()
{
	Color BackgroundColor(0.99, 0.99, 0.99);
	Color BorderColor(0.5, 0.5, 0.5);

	if (CheckHover() && CheckActive())
	{
		BackgroundColor[0] = 0.5;
		BackgroundColor[1] = 0.5;
		BackgroundColor[2] = 0.5;
		BorderColor[0] = 0.0;
		BorderColor[1] = 0.0;
		BorderColor[2] = 0.0;
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

	DrawBox(m_Position, m_Dimensions, BackgroundColor, BorderColor);
}

void ButtonWidget::ProcessTap()
{
	m_Action();
}
