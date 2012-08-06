#pragma once
#ifndef __ButtonWidget_H__
#define __ButtonWidget_H__

class ButtonWidget
	: public Widget
{
public:
	ButtonWidget(Vector2n Position, std::function<void()> Action);
	virtual ~ButtonWidget();

	void Render() override;
	
	void ProcessTap() override;

private:
	std::function<void()>		m_Action;
};

#endif // __ButtonWidget_H__
