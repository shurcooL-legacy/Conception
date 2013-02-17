#pragma once
#ifndef __GofmtWidget_H__
#define __GofmtWidget_H__

class GofmtWidget
	: public FlowLayoutWidget
{
public:
	GofmtWidget(Vector2n Position, TypingModule & TypingModule, Project & Project);
	~GofmtWidget();

private:
	GofmtWidget(const GofmtWidget &) = delete;
	GofmtWidget & operator = (const GofmtWidget &) = delete;

	ConnectionWidget<TextFieldWidget> * m_SourceWidget;
	TextFieldWidget * m_OutputWidget;
};

#endif // __GofmtWidget_H__
