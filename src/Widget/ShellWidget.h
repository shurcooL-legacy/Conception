#pragma once
#ifndef __ShellWidget_H__
#define __ShellWidget_H__

class ShellWidget
	: public FlowLayoutWidget
{
public:
	ShellWidget(Vector2n Position, TypingModule & TypingModule);
	~ShellWidget();

	void ProcessEvent(InputEvent & InputEvent) override;

private:
	ShellWidget(const ShellWidget &) = delete;
	ShellWidget & operator = (const ShellWidget &) = delete;

	TextFieldWidget * m_CommandWidget;
	ButtonWidget * m_ExecuteWidget;
	TextFieldWidget * m_OutputWidget;
};

#endif // __ShellWidget_H__
