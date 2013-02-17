#include "../Main.h"

LabelWidget::LabelWidget(const Vector2n Position, const std::function<std::string()> & Content, Background Background)
	: Widget(Position, Vector2n::ZERO, { /*std::shared_ptr<Behavior>(new DraggablePositionBehavior(*this))*/ }),
	  m_Content(Content),
	  m_Background(Background)
{
}

LabelWidget::LabelWidget(const Vector2n Position, const std::string & Content, Background Background)
	: LabelWidget(Position, [=]() { return Content; }, Background)
{
}

LabelWidget::~LabelWidget()
{
}

void LabelWidget::Render()
{
	// TODO: Refactor this out
	if (!m_Visible)
		return;

	Color BackgroundColor(1.0, 1.0, 1.0);
	Color BorderColor(0.6, 0.6, 0.6);

	auto Content = m_Content();
	ModifyDimensions() = Concept::GetDimensions(Content);

	if (Background::Normal == m_Background)
	{
		DrawAroundBox(GetPosition(), GetDimensions(), BackgroundColor, BorderColor);
	}

	OpenGLStream OpenGLStream(GetPosition());
	OpenGLStream << Content;
}

bool LabelWidget::IsHit(const Vector2n ParentPosition) const
{
	if (Background::None == m_Background)
	{
		return false;
	}
	else
	{
		return Widget::IsHit(ParentPosition);
	}
}
