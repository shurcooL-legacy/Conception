#pragma once
#ifndef __Widget_H__
#define __Widget_H__

class Widget
	: public GestureListener
{
public:
	virtual ~Widget();

	virtual void Render(/*WidgetManager & WidgetManager*/) = 0;

	virtual bool HitTest(Vector2n ParentPosition, std::list<Widget *> * Hits) const;

	virtual bool ShouldActivate() const;
	virtual bool ShouldDeactivate() const;

	//virtual bool ShouldMouseCursorVisible() const { return true; }

	virtual void ProcessActivation() {}
	virtual void ProcessDeactivation() {}

	//void SetActiveWidgetPointer(Widget ** ActiveWidgetPointer) { m_ActiveWidgetPointer = ActiveWidgetPointer; }
	/*void AddHoverPointer(Pointer * Pointer);
	void RemoveHoverPointer(Pointer * Pointer);*/

	virtual void ProcessCanvasUpdated();

	void SetParent(Canvas & Parent);
	const Canvas * GetParent() const;

	const GestureRecognizer & GetGestureRecognizer() { return m_GestureRecognizer; }
	GestureRecognizer & ModifyGestureRecognizer() { return m_GestureRecognizer; }

protected:
	Widget(Vector2n Position, Vector2n Dimensions);

	//bool IsActiveExternally() { return (this == *m_ActiveWidgetPointer); }

	virtual bool IsHit(const Vector2n ParentPosition) const;

	virtual const Vector2n ParentToLocal(const Vector2n ParentPosition) const;
	const Vector2n GlobalToParent(const Vector2n GlobalPosition) const;
	const Vector2n GlobalToLocal(const Vector2n GlobalPosition) const;

	bool CheckHover();
	bool CheckActive();

	Vector2n		m_Position;
	Vector2n		m_Dimensions;

private:
	//Widget **				m_ActiveWidgetPointer;
	//std::set<Pointer *>		m_HoverPointers;

	GestureRecognizer		m_GestureRecognizer;		// Owner of a single gesture recognizer

	Canvas *			m_Parent;

	friend class App;		// DEBUG: For debug printing
};

#endif // __Widget_H__
