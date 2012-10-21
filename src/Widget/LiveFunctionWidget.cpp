#include "../Main.h"

LiveFunctionWidget::LiveFunctionWidget(Vector2n Position, TypingModule & TypingModule)
	: CompositeWidget(Position, { std::shared_ptr<Widget>(new TextFieldWidget(Vector2n(0, 0), TypingModule)),
								  std::shared_ptr<Widget>(new TextFieldWidget(Vector2n(100, 0), TypingModule)) })
{
}

LiveFunctionWidget::~LiveFunctionWidget()
{
}
