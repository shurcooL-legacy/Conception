#pragma once
#ifndef __ButtonWidget_H__
#define __ButtonWidget_H__

class ButtonWidget
	: public CompositeWidget
{
public:
	ButtonWidget(Vector2n Position, Vector2n Dimensions, std::function<void()> Action, std::string Label = "");
	ButtonWidget(Vector2n Position, std::function<void()> Action)
		: ButtonWidget(Position, Vector2n(30, 30), Action) {}
	ButtonWidget(Vector2n Position, std::function<void()> Action, std::string Label);
	~ButtonWidget();

	void Render() override;

	//void ProcessTap(const InputEvent & InputEvent, Vector2n Position) override;
	void ProcessEvent(InputEvent & InputEvent) override;

	void SetAction(const std::function<void()> & Action);
	void Action();
	std::function<void()> & GetAction();

private:
	std::function<void()>		m_Action;
};

#endif // __ButtonWidget_H__
