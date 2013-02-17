#pragma once
#ifndef __LiveGofmtWidget_H__
#define __LiveGofmtWidget_H__

class LiveGofmtWidget
	: public FlowLayoutWidget
{
public:
	LiveGofmtWidget(Vector2n Position, TypingModule & TypingModule);
	~LiveGofmtWidget();

private:
	LiveGofmtWidget(const LiveGofmtWidget &) = delete;
	LiveGofmtWidget & operator = (const LiveGofmtWidget &) = delete;

	TextFieldWidget * m_SourceWidget;
	GofmtWidget * m_GofmtWidget;
};

#endif // __LiveGofmtWidget_H__
