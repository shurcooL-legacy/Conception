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

	void ProcessManipulationBegin(const PointerState & PointerState) override;
	void ProcessManipulationUpdate(const PointerState & PointerState) override;
	void ProcessManipulationEnd(const PointerState & PointerState) override;

	void ProcessTimePassed(const double TimePassed) override;

private:
	uint8					m_Color;

	// DEBUG: For info printing
	friend class App;
	friend class MultitouchTestApp;
	friend class ConceptionApp;
};

#endif // __MultitouchTestBoxWidget_H__
