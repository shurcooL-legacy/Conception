#pragma once
#ifndef __LiveRubyWidget_H__
#define __LiveRubyWidget_H__

class LiveRubyWidget
	: public FlowLayoutWidget
{
public:
	LiveRubyWidget(Vector2n Position, TypingModule & TypingModule);
	~LiveRubyWidget();

private:
	LiveRubyWidget(const LiveRubyWidget &) = delete;
	LiveRubyWidget & operator = (const LiveRubyWidget &) = delete;

	TextFieldWidget * m_SourceWidget;
	RubyWidget * m_RubyWidget;
};

#endif // __LiveRubyWidget_H__
