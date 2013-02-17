#pragma once
#ifndef __ConnectionWidget_H__
#define __ConnectionWidget_H__

template <typename T> class ConnectionWidget
	: public Widget,
	  public MutuallyConnectable<ConnectionWidget<T>, T>
{
public:
	ConnectionWidget(Vector2n Position, T * Target = nullptr);
	~ConnectionWidget();

	void Render() override;

	void ProcessEvent(InputEvent & InputEvent) override;

	// HACK: void ProcessTimePassed(const double TimePassed) override { if (nullptr != m_OnChange) m_OnChange(); };

	T * Target() const { return m_Target; }

	void NotifyChange() const;
	std::function<void()> m_OnChange = nullptr;

private:
	void UpdateTarget(T * Target);

	T * m_Target = nullptr;
};

template <typename T> ConnectionWidget<T>::ConnectionWidget(Vector2n Position, T * Target)
	: Widget(Position, Vector2n(16, 16), { /*std::shared_ptr<Behavior>(new NonDraggablePositionBehavior(*this))*/ }),
	  m_Target(nullptr)		// It will actually be set inside the constructor, via a call to UpdateTarget(Target), but m_Target has to be nullptr at top of UpdateTarget()
{
	UpdateTarget(Target);
}

template <typename T> ConnectionWidget<T>::~ConnectionWidget()
{
}

template <typename T> void ConnectionWidget<T>::UpdateTarget(T * Target)
{
	if (nullptr != m_Target)
		MutuallyConnectable<ConnectionWidget<T>, T>::Disconnect(*this, *m_Target);

	m_Target = Target;

	if (nullptr != m_Target)
		MutuallyConnectable<ConnectionWidget<T>, T>::Connect(*this, *m_Target);

	if (nullptr != m_OnChange) {
		m_OnChange();
	}
}

template <typename T> void ConnectionWidget<T>::NotifyChange() const
{
	if (nullptr != m_OnChange) {
		m_OnChange();
	}
}

template <typename T> void ConnectionWidget<T>::Render()
{
	// TODO: Refactor this out
	if (!m_Visible)
		return;

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

	if (CheckActive())
	{
		for (auto & Pointer : GetGestureRecognizer().GetConnected())
		{
			if (Pointer::VirtualCategory::POINTING == Pointer->GetVirtualCategory())
			{
				const auto & PointerState = Pointer->GetPointerState();
				Vector2n GlobalPosition(PointerState.GetAxisState(0).GetPosition(), PointerState.GetAxisState(1).GetPosition());
				Vector2n LocalPosition = GlobalToLocal(GlobalPosition);

				// TODO: Replace this with a resolution independent quad-line
				glColor3dv(BorderColor.GetComponents());
				glBegin(GL_LINES);
					glVertex2i(Center.X(), Center.Y());
					glVertex2i(GetPosition().X() + LocalPosition.X(), GetPosition().Y() + LocalPosition.Y());
				glEnd();
			}
		}
	} else if (nullptr != m_Target) {
		Vector2n GlobalPosition = m_Target->LocalToGlobal(m_Target->GetDimensions() / 2);
		Vector2n LocalPosition = GlobalToLocal(GlobalPosition);

		// TODO: Replace this with a resolution independent quad-line
		glColor3dv(BorderColor.GetComponents());
		glBegin(GL_LINES);
			glVertex2i(Center.X(), Center.Y());
			glVertex2i(GetPosition().X() + LocalPosition.X(), GetPosition().Y() + LocalPosition.Y());
		glEnd();
	}

	DrawCircle(Center, GetDimensions(), BackgroundColor, BorderColor);
	DrawCircle(Center, Vector2n(6, 6), BorderColor, BorderColor);
}

template <typename T> void ConnectionWidget<T>::ProcessEvent(InputEvent & InputEvent)
{
	if (IsPointerButtonEvent<Pointer::VirtualCategory::POINTING, 0, false>(InputEvent))
	{
		T * Target = nullptr;
		if (nullptr != InputEvent.m_Pointer->m_CurrentPointerMappingTEST.GetHoverer())
			Target = dynamic_cast<T *>(&InputEvent.m_Pointer->m_CurrentPointerMappingTEST.GetHoverer()->GetOwner());

		if (m_Target != Target)
		{
			UpdateTarget(Target);
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
