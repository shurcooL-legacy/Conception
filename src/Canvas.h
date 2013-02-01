#pragma once
#ifndef __Canvas_H__
#define __Canvas_H__

class Canvas
	: public CompositeWidget
{
public:
	enum class BehaviourScrolling : uint8 {
		Zooming,
		Freeform,
		VerticalOnly
	};

	Canvas(Vector2n Position, bool Centered, bool HasBackground, BehaviourScrolling BehaviourScrolling = BehaviourScrolling::Zooming);
	~Canvas();

	void Render() override;

	bool HitTest(Vector2n ParentPosition, std::list<Widget *> * Hits) const;

	const Vector2n GetDimensions() const override;

	void ProcessTap(const InputEvent & InputEvent, Vector2n Position) override;

	/*void ProcessButton(Pointer * Pointer, Input::InputId ButtonId, bool Pressed);
	void ProcessSlider(Pointer * Pointer, Input::InputId SliderId, double MovedAmount);*/
	//void ProcessDrag(Vector2d DragAmount) override;
	void ProcessScroll(InputEvent & InputEvent, Vector2n ScrollAmount) override;

	void ProcessManipulationBegin(const PointerState & PointerState) override;
	void ProcessManipulationUpdate(const PointerState & PointerState) override;
	void ProcessManipulationEnd(const PointerState & PointerState) override;

	void ProcessTimePassed(const double TimePassed) override;

	void ProcessCanvasUpdated() override;

	void MoveView(uint8 Degree, double MoveAmount, double A[2] = nullptr, Vector2n ParentLocalPosition = Vector2n::ZERO);

	bool m_BlackBackgroundTEST;

protected:
	bool IsHit(const Vector2n ParentPosition) const override;

private:
	Canvas(const Canvas &) = delete;
	Canvas & operator = (const Canvas &) = delete;

	void RenderBackground();
	const Rectanglen GetScissorBox() const;
	void SetScissorBox(Rectanglen ScissorBox);
	void SetupTransform();
	void EndTransform();

	//const bool GlobalIsInsideCanvas(const Vector2n Position) const;
	//const Vector2d GlobalToCanvas(const Vector2n Position) const;

	virtual const Vector2n ParentToLocal(const Vector2n ParentPosition) const override;
	//virtual const Vector2n GlobalToLocal(const Vector2n GlobalPosition) const override;		Got replaced by ParentToLocal()

	Vector2d Camera;
	double CameraZ;

	bool		m_Centered;
	bool		m_HasBackground;

	Rectanglen	m_ScissorBox;

	BehaviourScrolling		m_BehaviourScrolling;

	//std::vector<std::unique_ptr<Widget>>		m_Widgets;
};

#endif // __Canvas_H__
