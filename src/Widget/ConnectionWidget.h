#pragma once
#ifndef __ConnectionWidget_H__
#define __ConnectionWidget_H__

template <typename T> class ConnectionWidget
	: public Widget
{
public:
	ConnectionWidget(Vector2n Position, T * Target);
	~ConnectionWidget();

	void Render() override;

	void ProcessEvent(InputEvent & InputEvent) override;

	T * Target() const { return m_Target; }

private:
	T * m_Target = nullptr;
};

template <typename T> ConnectionWidget<T>::ConnectionWidget(Vector2n Position, T * Target)
	: Widget(Position, Vector2n(14, 14), { /*std::shared_ptr<Behavior>(new NonDraggablePositionBehavior(*this))*/ }),
	  m_Target(Target)
{
}

template <typename T> ConnectionWidget<T>::~ConnectionWidget()
{
}

template <typename T> void ConnectionWidget<T>::Render()
{
	Color BackgroundColor(0.99, 0.99, 0.99);
	Color BorderColor(0.5, 0.5, 0.5);

	if (CheckActive())
	{
		BackgroundColor[0] = 0.75;
		BackgroundColor[1] = 0.75;
		BackgroundColor[2] = 0.75;
		BorderColor[0] = 0.898;
		BorderColor[1] = 0.765;
		BorderColor[2] = 0.396;
	}
	//else if ((CheckHover() && !CheckAnyActive()) || (!CheckHover() && CheckActive()))
	else if (CheckHover() && !CheckActive())
	{
		BackgroundColor[0] = 1.0;
		BackgroundColor[1] = 1.0;
		BackgroundColor[2] = 1.0;
		BorderColor[0] = 0.898;
		BorderColor[1] = 0.765;
		BorderColor[2] = 0.396;
	}
	else
	{
	}

	auto Center = GetPosition() + GetDimensions() / 2;

	DrawCircle(Center, GetDimensions(), BackgroundColor, BorderColor);
	DrawCircle(Center, Vector2n(6, 6), BorderColor, BorderColor);

	if (CheckActive())
	{
		for (auto & Pointer : GetGestureRecognizer().GetConnected())
		{
			if (Pointer::VirtualCategory::POINTING == Pointer->GetVirtualCategory())
			{
				const auto & PointerState = Pointer->GetPointerState();
				Vector2n GlobalPosition(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition());
				Vector2n ParentPosition = GlobalToParent(GlobalPosition);
				Vector2n LocalPosition = ParentToLocal(ParentPosition);

				// TODO: Replace this with a resolution independent quad-line
				glColor3dv(BorderColor.GetComponents());
				glBegin(GL_LINES);
					glVertex2i(Center.X(), Center.Y());
					glVertex2i(GetPosition().X() + LocalPosition.X(), GetPosition().Y() + LocalPosition.Y());
				glEnd();
			}
		}
	}

	//std::cout << m_Target << endl;
}

template <typename T> void ConnectionWidget<T>::ProcessEvent(InputEvent & InputEvent)
{
	if (IsPointerButtonEvent<Pointer::VirtualCategory::POINTING, 0, false>(InputEvent))
	{
		printf("Released\n");
		if (nullptr != InputEvent.m_Pointer->m_CurrentPointerMappingTEST.GetHoverer())
		{
			m_Target = dynamic_cast<T *>(&InputEvent.m_Pointer->m_CurrentPointerMappingTEST.GetHoverer()->GetOwner());
		}

		InputEvent.m_Handled = true;
	}

	// HACK: Block all events other than vertical axis scroll events
	if (!(   InputEvent.HasType(InputEvent::EventType::AXIS_EVENT)
		  && 2 == InputEvent.m_InputId))
	{
		InputEvent.m_Handled = true;
	}
}

#endif // __ConnectionWidget_H__
