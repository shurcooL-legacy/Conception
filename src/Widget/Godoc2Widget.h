#pragma once
#ifndef __Godoc2Widget_H__
#define __Godoc2Widget_H__

class Godoc2Widget
	: public FlowLayoutWidget
{
public:
	Godoc2Widget(Vector2n Position, TypingModule & TypingModule, TextFieldWidget * Target = nullptr);
	~Godoc2Widget();

private:
	Godoc2Widget(const Godoc2Widget &) = delete;
	Godoc2Widget & operator = (const Godoc2Widget &) = delete;

	ConnectionWidget<TextFieldWidget> * m_SourceWidget;
	TextFieldWidget * m_OutputWidget;
	std::unique_ptr<ShellWidget> m_ShellWidget;
};

#endif // __Godoc2Widget_H__
