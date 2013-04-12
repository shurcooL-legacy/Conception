#pragma once
#ifndef __Widget_H__
#define __Widget_H__

class Widget
	: public GestureHandler
{
public:
	virtual ~Widget();

	void AddBehavior(Behavior * Behavior);
	void RemoveAllBehaviors();

	virtual void Render() = 0;

	virtual MatchResult MatchEventQueue(InputEventQueue::FilteredQueue & UnreservedEvents);

	virtual bool HitTest(Vector2n ParentPosition, std::list<Widget *> * Hits) const;
	virtual void HitTest2(Vector2n ParentPosition, PointerMapping & Mapping) const;		// REWRITE of HitTest()
	virtual bool IsHit(const Vector2n ParentPosition) const;		// TODO: Made it public cuz of some problem with CanvasWidget::IsHit(), see if I can make it protected once again

	//virtual bool ShouldMouseCursorVisible() const { return true; }

	//void SetActiveWidgetPointer(Widget ** ActiveWidgetPointer) { m_ActiveWidgetPointer = ActiveWidgetPointer; }
	/*void AddHoverPointer(Pointer * Pointer);
	void RemoveHoverPointer(Pointer * Pointer);*/

	virtual void ProcessCanvasUpdated();

	const CompositeWidget * GetParent() const;
	CompositeWidget * ModifyParent();

	const GestureRecognizer & GetGestureRecognizer() const { return m_GestureRecognizer; }
	GestureRecognizer & ModifyGestureRecognizer() { return m_GestureRecognizer; }

	const Vector2n GetPosition() const;
	virtual const Vector2n GetDimensions() const;
	Vector2n & ModifyPosition();
	Vector2n & ModifyDimensions();
	void SetPosition(Vector2n Position);
	void SetDimensions(Vector2n Dimensions);

	virtual const Vector2n LocalToParent(const Vector2n LocalPosition) const;		// TODO: This should be one reversable virtual function, not two (i.e. DRY)
	const Vector2n ParentToGlobal(const Vector2n ParentPosition) const;
	const Vector2n LocalToGlobal(const Vector2n LocalPosition) const;

	void ProcessManipulationBegin(const InputEvent & InputEvent) override;
	void ProcessManipulationUpdate(const InputEvent & InputEvent) override;
	void ProcessManipulationEnd(const InputEvent & InputEvent) override;

	void ProcessTimePassed(const double TimePassed) override;

	bool m_Visible;

	std::string m_DebugDescription = "";

protected:
	Widget(Vector2n Position, Vector2n Dimensions, std::vector<std::shared_ptr<Behavior>> Behaviors);

	//bool IsActiveExternally() { return (this == *m_ActiveWidgetPointer); }

	virtual const Vector2n ParentToLocal(const Vector2n ParentPosition) const;
	const Vector2n GlobalToParent(const Vector2n GlobalPosition) const;
public:const Vector2n GlobalToLocal(const Vector2n GlobalPosition) const;protected:

	bool CheckHover() const;
	bool CheckActive() const;
	bool HasTypingFocus() const;
	bool HasTopmostTypingFocus() const;
	bool HasAnyTypingFocus() const;

private:
	Widget(const Widget &) = delete;
	Widget & operator = (const Widget &) = delete;

	Vector2n		m_Position;
	Vector2n		m_Dimensions;

	//Widget **				m_ActiveWidgetPointer;
	//std::set<Pointer *>		m_HoverPointers;

	GestureRecognizer		m_GestureRecognizer;		// Owner of a single gesture recognizer

	std::vector<GestureType>	m_GestureTypes;

	std::vector<std::shared_ptr<Behavior>>			m_Behaviors;

	CompositeWidget * m_Parent = nullptr;

	void SetParent(CompositeWidget & Parent);

	friend class CompositeWidget;				// For SetParent() access
	friend class DraggablePositionBehavior;		// For GlobalToParent() access
	friend class GestureRecognizer;				// For GlobalToParent() access

	// DEBUG: For info printing
	friend class DebugOverlayWidget;
};

#endif // __Widget_H__
