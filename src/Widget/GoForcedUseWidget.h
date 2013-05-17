#pragma once
#ifndef __GoForcedUseWidget_H__
#define __GoForcedUseWidget_H__

class GoForcedUseWidget
	: public FlowLayoutWidget
{
public:
	GoForcedUseWidget(Vector2n Position, TypingModule & TypingModule, TextFieldWidget * Target = nullptr);
	~GoForcedUseWidget();

private:
	GoForcedUseWidget(const GoForcedUseWidget &) = delete;
	GoForcedUseWidget & operator = (const GoForcedUseWidget &) = delete;

	ConnectionWidget<TextFieldWidget> * m_SourceWidget;
	TextFieldWidget * m_OutputWidget;
	std::unique_ptr<ShellWidget> m_ShellWidget;
};

#endif // __GoForcedUseWidget_H__
