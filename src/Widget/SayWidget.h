#pragma once
#ifndef __SayWidget_H__
#define __SayWidget_H__

class SayWidget
	: public FlowLayoutWidget
{
public:
	SayWidget(Vector2n Position, TypingModule & TypingModule);
	~SayWidget();

private:
	SayWidget(const SayWidget &) = delete;
	SayWidget & operator = (const SayWidget &) = delete;

	TextFieldWidget * m_InputWidget;
	ButtonWidget * m_ExecuteWidget;
};

#endif // __SayWidget_H__
