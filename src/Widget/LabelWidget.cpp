#include "../Main.h"

LabelWidget::LabelWidget(const Vector2n Position, const std::function<std::string()> & Content)
	: Widget(Position, Vector2n::ZERO, { /*std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this))*/ }),
	  m_Content(Content)
{
}

LabelWidget::~LabelWidget()
{
}

void LabelWidget::Render()
{
	Color BackgroundColor(1.0, 1.0, 1.0);
	Color BorderColor(0.3, 0.3, 0.3);

	// TODO: Add resizing code, etc.
	//DrawAroundBox(GetPosition(), GetDimensions(), BackgroundColor, BorderColor);

	OpenGLStream OpenGLStream(GetPosition());
	OpenGLStream << m_Content();
}
