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

	void ProcessTap() override;

	/*void ProcessButton(Pointer * Pointer, Input::InputId ButtonId, bool Pressed);
	void ProcessSlider(Pointer * Pointer, Input::InputId SliderId, double MovedAmount);*/
	//void ProcessDrag(Vector2d DragAmount) override;

	void ProcessManipulationStarted(const PointerState & PointerState) override;
	void ProcessManipulationUpdated(const PointerState & PointerState) override;
	void ProcessManipulationCompleted(const PointerState & PointerState) override;

	void ProcessTimePassed(const double TimePassed) override;

private:
	uint8					m_Color;

	friend class App;		// DEBUG: For debug printing
};

#endif // __MultitouchTestBoxWidget_H__
