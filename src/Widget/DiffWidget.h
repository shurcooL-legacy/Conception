#pragma once
#ifndef __DiffWidget_H__
#define __DiffWidget_H__

class DiffWidget
	: public FlowLayoutWidget
{
public:
	DiffWidget(Vector2n Position, TypingModule & TypingModule, TextFieldWidget * Target1 = nullptr, TextFieldWidget * Target2 = nullptr);
	~DiffWidget();

private:
	DiffWidget(const DiffWidget &) = delete;
	DiffWidget & operator = (const DiffWidget &) = delete;

	ConnectionWidget<TextFieldWidget> * m_Source1Widget;
	ConnectionWidget<TextFieldWidget> * m_Source2Widget;
	TextFieldWidget * m_OutputWidget;
};

#endif // __DiffWidget_H__
