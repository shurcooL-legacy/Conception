#include "../Main.h"

LabelWidget::LabelWidget(Vector2n Position, std::string Content)
	: Widget(Position, Vector2n(30, 50)),
	  m_Content(Content)
{
}

LabelWidget::~LabelWidget()
{
}

void LabelWidget::Render()
{
	OpenGLStream(GetPosition()) << m_Content;
}
