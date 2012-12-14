#pragma once
#ifndef __LiveFunctionWidget_H__
#define __LiveFunctionWidget_H__

class LiveFunctionWidget
	: public FlowLayoutWidget
{
public:
	LiveFunctionWidget(Vector2n Position, TypingModule & TypingModule, Project & Project);
	~LiveFunctionWidget();

private:
	LiveFunctionWidget(const LiveFunctionWidget &) = delete;
	LiveFunctionWidget & operator = (const LiveFunctionWidget &) = delete;

	TextFieldWidget * m_SourceWidget;
	TextFieldWidget * m_OutputWidget;

	Project & m_Project;
};

#endif // __LiveFunctionWidget_H__
