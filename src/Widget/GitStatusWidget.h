#pragma once
#ifndef __GitStatusWidget_H__
#define __GitStatusWidget_H__

class GitStatusWidget
	: public FlowLayoutWidget
{
public:
	GitStatusWidget(Vector2n Position, TypingModule & TypingModule, TextFileWidget * Target = nullptr);
	~GitStatusWidget();

private:
	GitStatusWidget(const GitStatusWidget &) = delete;
	GitStatusWidget & operator = (const GitStatusWidget &) = delete;

	ConnectionWidget<TextFileWidget> * m_SourceWidget;
	TextFieldWidget * m_OutputWidget;
};

#endif // __GitStatusWidget_H__
