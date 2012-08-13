#pragma once
#ifndef __FunctionWidget_H__
#define __FunctionWidget_H__

class FunctionWidget
	: public Widget
{
public:
	FunctionWidget(Vector2n Position, Function & Function);
	virtual ~FunctionWidget();

	void Render() override;

	void ProcessTap() override;
	void ProcessCharacter(InputEvent & InputEvent, const uint32 Character) override;

	void ProcessEvent(InputEvent & InputEvent) override;

private:
	Function & m_Function;
};

#endif // __FunctionWidget_H__
