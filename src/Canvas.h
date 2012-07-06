#pragma once
#ifndef __Canvas_H__
#define __Canvas_H__

class Canvas
	: public Widget
{
public:
	Canvas(Vector2n Position, bool Centered, bool HasBackground);
	~Canvas();

	void Render();

	void AddWidget(Widget * Widget);

	bool HitTest(Vector2n ParentPosition, std::list<Widget *> * Hits) const;
	
	void ProcessTap() override;

	/*void ProcessButton(Pointer * Pointer, Input::InputId ButtonId, bool Pressed);
	void ProcessSlider(Pointer * Pointer, Input::InputId SliderId, double MovedAmount);*/
	//void ProcessDrag(Vector2d DragAmount) override;
	void ProcessScroll(InputEvent & InputEvent, Vector2n ScrollAmount) override;

	void ProcessManipulationStarted(const PointerState & PointerState) override;
	void ProcessManipulationUpdated(const PointerState & PointerState) override;
	void ProcessManipulationCompleted(const PointerState & PointerState) override;

	void ProcessTimePassed(const double TimePassed) override;

	void ProcessCanvasUpdated() override;

	void MoveView(uint8 Degree, double MoveAmount, double A[2] = 0, Vector2n ParentLocalPosition = Vector2n::ZERO);
	void RenderBackground();
	const Rectanglen GetScissorBox() const;
	void SetScissorBox(Rectanglen ScissorBox);
	void SetupTransform();
	void EndTransform();

	//const bool GlobalIsInsideCanvas(const Vector2n Position) const;
	//const Vector2d GlobalToCanvas(const Vector2n Position) const;

	virtual const Vector2n ParentToLocal(const Vector2n ParentPosition) const override;
	//virtual const Vector2n GlobalToLocal(const Vector2n GlobalPosition) const override;		Got replaced by ParentToLocal()

	void SetDimensions(Vector2n Dimensions);

	bool m_BlackBackgroundTEST;

private:
	Canvas(const Canvas &) = delete;
	Canvas & operator =(const Canvas &) = delete;

	Vector2d Camera;
	double CameraZ;

	bool		m_Centered;
	bool		m_HasBackground;

	Rectanglen	m_ScissorBox;

	std::vector<std::unique_ptr<Widget>>		m_Widgets;
};

#endif // __Canvas_H__
