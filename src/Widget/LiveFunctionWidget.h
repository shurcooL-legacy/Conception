#pragma once
#ifndef __LiveFunctionWidget_H__
#define __LiveFunctionWidget_H__

class LiveFunctionWidget
	: public CompositeWidget
{
public:
	LiveFunctionWidget(Vector2n Position, TypingModule & TypingModule, Project & Project);
	~LiveFunctionWidget();

private:
	LiveFunctionWidget(const LiveFunctionWidget &) = delete;
	LiveFunctionWidget & operator = (const LiveFunctionWidget &) = delete;

	TextFieldWidget * m_InputWidget;
	TextFieldWidget * m_SourceWidget;
	ToggleWidget * m_ToggleWidget;
	LiveProgramWidget * m_LiveProgramWidget;
};

#endif // __LiveFunctionWidget_H__
