#pragma once
#ifndef __RubyWidget_H__
#define __RubyWidget_H__

class RubyWidget
	: public FlowLayoutWidget
{
public:
	RubyWidget(Vector2n Position, TypingModule & TypingModule, TextFieldWidget * Target = nullptr);
	~RubyWidget();

private:
	RubyWidget(const RubyWidget &) = delete;
	RubyWidget & operator = (const RubyWidget &) = delete;

	ConnectionWidget<TextFieldWidget> * m_SourceWidget;
	TextFieldWidget * m_OutputWidget;
};

#endif // __RubyWidget_H__
