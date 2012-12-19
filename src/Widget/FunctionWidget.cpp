#include "../Main.h"

FunctionWidget::FunctionWidget(Vector2n Position, Function & Function)
	: Widget(Position, Vector2n(904, (3 + 2/*f.body_lines.size()*/) * lineHeight), {}),
	  m_Function(Function)
{
	ModifyGestureRecognizer().m_RecognizeTap = true;
}

FunctionWidget::~FunctionWidget()
{
}

void FunctionWidget::Render()
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
	//if (CheckHover())
	// HACK
	if (HasTypingFocus())
	{
		BorderColor[0] = 0.898;
		BorderColor[1] = 0.765;
		BorderColor[2] = 0.396;
	}

	DrawAroundBox(GetPosition(), GetDimensions(), BackgroundColor, BorderColor);

	glColor3d(0, 0, 0); OglUtilsPrint(GetPosition().X(), GetPosition().Y() - lineHeight, 0, LEFT, "Global Function");

	OpenGLStream OpenGLStream(GetPosition());
	OpenGLStream << m_Function;
}

void FunctionWidget::ProcessTap(const InputEvent & InputEvent, Vector2n Position)
{
	g_InputManager->RequestTypingPointer(ModifyGestureRecognizer());
}

void FunctionWidget::ProcessCharacter(InputEvent & InputEvent, const uint32 Character)
{
}

void FunctionWidget::ProcessEvent(InputEvent & InputEvent)
{
}
