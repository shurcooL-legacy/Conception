#pragma once
#ifndef __ButtonWidget_H__
#define __ButtonWidget_H__

class ButtonWidget
	: public Widget
{
public:
	ButtonWidget(Vector2n Position, Vector2n Dimensions, std::function<void()> Action);
	ButtonWidget(Vector2n Position, std::function<void()> Action)
		: ButtonWidget(Position, Vector2n(30, 30), Action) {}
	virtual ~ButtonWidget();

	void Render() override;
	
	void ProcessTap() override;

private:
	std::function<void()>		m_Action;
};

#endif // __ButtonWidget_H__
