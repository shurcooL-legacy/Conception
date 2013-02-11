#pragma once
#ifndef __SayWidget_H__
#define __SayWidget_H__

class SayWidget
	: public FlowLayoutWidget
{
public:
	SayWidget(Vector2n Position, TypingModule & TypingModule);
	~SayWidget();

	TextFieldWidget * m_InputWidget;
	ButtonWidget * m_ExecuteWidget;

private:
	SayWidget(const SayWidget &) = delete;
	SayWidget & operator = (const SayWidget &) = delete;
};

#endif // __SayWidget_H__
