#pragma once
#ifndef __MultitouchTestBoxWidget_H__
#define __MultitouchTestBoxWidget_H__

class MultitouchTestBoxWidget
	: public Widget
{
public:
	MultitouchTestBoxWidget(Vector2n Position);
	virtual ~MultitouchTestBoxWidget();

	void Render();

	void ProcessTap(const InputEvent & InputEvent, Vector2n Position) override;

	/*void ProcessButton(Pointer * Pointer, Input::InputId ButtonId, bool Pressed);
	void ProcessSlider(Pointer * Pointer, Input::InputId SliderId, double MovedAmount);*/
	//void ProcessDrag(Vector2d DragAmount) override;

	void ProcessManipulationBegin(const InputEvent & InputEvent) override;
	void ProcessManipulationUpdate(const InputEvent & InputEvent) override;
	void ProcessManipulationEnd(const InputEvent & InputEvent) override;

	void ProcessTimePassed(const double TimePassed) override;

private:
	uint8					m_Color;

	// DEBUG: For info printing
	friend class DebugOverlayWidget;
};

#endif // __MultitouchTestBoxWidget_H__
