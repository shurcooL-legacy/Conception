#include "../Main.h"

Widget::Widget(Vector2n Position, Vector2n Dimensions)
	: m_Position(Position),
	  m_Dimensions(Dimensions),
	  //m_HoverPointers()
	  m_GestureRecognizer(*this),
	  m_Parent(nullptr)
{
}

Widget::~Widget()
{
}

bool Widget::HasTypingFocus() const
{
	return (GetGestureRecognizer().GetConnected().end() != GetGestureRecognizer().GetConnected().find(g_InputManager->m_TypingPointer.get()));
}

bool Widget::CheckHover() const
{
	//return !m_HoverPointers.empty();
	//return false;
	return (!GetGestureRecognizer().GetConnected().empty());		// TEST
}

bool Widget::CheckActive() const
{
	//return (this == WidgetManager.m_ActiveWidget);
	/*for (auto Pointer : m_HoverPointers)
	{
		//if (this == Pointer->GetWidgetMapping().GetActiveWidget())
		{
			return true;
		}
	}*/

	return false;
}

void Widget::ProcessCanvasUpdated()
{
	/*if (nullptr != m_GestureRecognizerTEST)
	{
		m_GestureRecognizerTEST->ProcessCanvasUpdated();
	}*/
	ModifyGestureRecognizer().ProcessCanvasUpdated();
}

void Widget::SetParent(Widget & Parent)
{
	m_Parent = &Parent;
}

const Widget * Widget::GetParent() const
{
	return m_Parent;
}

// Returns true if there is any (even partial) hit
// Returns false if there is no hit whatsoever
bool Widget::HitTest(Vector2n ParentPosition, std::list<Widget *> * Hits) const
{
	auto Hit = IsHit(ParentPosition);

	if (Hit && nullptr != Hits)
	{
		Hits->push_back(const_cast<Widget *>(this));
	}

	return Hit;
}

bool Widget::IsHit(const Vector2n ParentPosition) const
{
	bool Hit = (   ParentPosition.X() >= m_Position.X()
				&& ParentPosition.Y() >= m_Position.Y()
				&& ParentPosition.X() <  m_Position.X() + m_Dimensions.X()
				&& ParentPosition.Y() <  m_Position.Y() + m_Dimensions.Y());

	return Hit;
}

const Vector2n Widget::ParentToLocal(const Vector2n ParentPosition) const
{
	return ParentPosition - m_Position;
}

const Vector2n Widget::GlobalToParent(const Vector2n GlobalPosition) const
{
	Vector2n ParentPosition = GlobalPosition;

	if (nullptr != GetParent())
	{
		ParentPosition = GetParent()->GlobalToLocal(ParentPosition);
	}

	return ParentPosition;
}

const Vector2n Widget::GlobalToLocal(const Vector2n GlobalPosition) const
{
	/*Vector2d CanvasPositionDouble = GetParent()->GlobalToCanvas(GlobalPosition);
	Vector2n CanvasPosition(std::lround(CanvasPositionDouble.X()), std::lround(CanvasPositionDouble.Y()));		// TODO: Loss of accuracy? Fix it if needed.

	return CanvasPosition - m_Position;*/

	return ParentToLocal(GlobalToParent(GlobalPosition));
}

const Vector2n Widget::GetPosition() const
{
	return m_Position;
}

const Vector2n Widget::GetDimensions() const
{
	return m_Dimensions;
}

void Widget::SetPosition(Vector2n Position)
{
	m_Position = Position;
}

void Widget::SetDimensions(Vector2n Dimensions)
{
	m_Dimensions = Dimensions;
}

Vector2n & Widget::ModifyPosition()
{
	return m_Position;
}

Vector2n & Widget::ModifyDimensions()
{
	return m_Dimensions;
}

/*void Widget::AddHoverPointer(Pointer * Pointer)
{
	m_HoverPointers.insert(Pointer);
}

void Widget::RemoveHoverPointer(Pointer * Pointer)
{
	m_HoverPointers.erase(Pointer);
}*/
