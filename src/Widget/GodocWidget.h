#pragma once
#ifndef __GodocWidget_H__
#define __GodocWidget_H__

class GodocWidget
	: public FlowLayoutWidget
{
public:
	GodocWidget(Vector2n Position, TypingModule & TypingModule, TextFieldWidget * Target = nullptr);
	~GodocWidget();

private:
	GodocWidget(const GodocWidget &) = delete;
	GodocWidget & operator = (const GodocWidget &) = delete;

	ConnectionWidget<TextFieldWidget> * m_SourceWidget;
	TextFieldWidget * m_OutputWidget;
	std::unique_ptr<ShellWidget> m_ShellWidget;
};

#endif // __GodocWidget_H__
