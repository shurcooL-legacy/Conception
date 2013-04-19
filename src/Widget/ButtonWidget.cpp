#include "../Main.h"

ButtonWidget::ButtonWidget(Vector2n Position, std::function<void()> Action, std::string Label)
	: ButtonWidget(Position, Vector2n::ZERO, Action, Label)
{}

ButtonWidget::ButtonWidget(Vector2n Position, Vector2n Dimensions, std::function<void()> Action, std::string Label)
	: CompositeWidget(Position, Dimensions, { /*std::shared_ptr<Behavior>(new NonDraggablePositionBehavior(*this))*/ }, {}),
	  m_Action(Action)
{
	assert(nullptr != m_Action);

	//ModifyGestureRecognizer().m_RecognizeTap = true;

	if (!Label.empty())
	{
		auto Offset = Vector2n(4, 2);
		auto x = new LabelWidget(Offset, Label);
		ModifyDimensions() = x->GetDimensions() + Offset * 2;
		AddWidget(x);
	}
}

ButtonWidget::~ButtonWidget()
{
}

void ButtonWidget::Render()
{
	Color BackgroundColor(0.99, 0.99, 0.99);
	Color BorderColor(0.5, 0.5, 0.5);

	if (CheckHover() && CheckActive())
	{
		BackgroundColor[0] = 0.75;
		BackgroundColor[1] = 0.75;
		BackgroundColor[2] = 0.75;
		BorderColor[0] = 0.898;
		BorderColor[1] = 0.765;
		BorderColor[2] = 0.396;
	}
	//else if ((CheckHover() && !CheckAnyActive()) || (!CheckHover() && CheckActive()))
	else if (CheckHover() ^ CheckActive())
	{
		BackgroundColor[0] = 1.0;
		BackgroundColor[1] = 1.0;
		BackgroundColor[2] = 1.0;
		BorderColor[0] = 0.898;
		BorderColor[1] = 0.765;
		BorderColor[2] = 0.396;
	}

	DrawInnerBox(GetPosition(), GetDimensions(), BackgroundColor, BorderColor);

	CompositeWidget::Render();
}

/*void ButtonWidget::ProcessTap(const InputEvent & InputEvent, Vector2n Position)
{
	m_Action();
}*/

void ButtonWidget::ProcessEvent(InputEvent & InputEvent)
{
	Vector2n GlobalPosition(InputEvent.m_PostEventState.GetAxisState(0).GetPosition(), InputEvent.m_PostEventState.GetAxisState(1).GetPosition());

	if (   IsPointerButtonEvent<Pointer::VirtualCategory::POINTING, 0, false>(InputEvent)
		&& &GetGestureRecognizer() == InputEvent.m_Pointer->m_CurrentPointerMappingTEST.GetHoverer()		// IsHit(this button) should be true
		&& &GetGestureRecognizer() == InputEvent.m_Pointer->GetPointerMapping().GetHoverer())		// Make sure we're releasing pointer over same button that it originally went active on, and nothing is in the way (i.e. button is hoverer)
	{
		m_Action();

		InputEvent.m_Handled = true;
	}

	// HACK: Block all events other than vertical axis scroll events
	if (!(   InputEvent.HasType(InputEvent::EventType::AXIS_EVENT)
		  && 2 == InputEvent.m_InputId))
	{
		InputEvent.m_Handled = true;
	}
}

void ButtonWidget::SetAction(const std::function<void()> & Action)
{
	m_Action = Action;
}

void ButtonWidget::Action()
{
	m_Action();
}

std::function<void()> & ButtonWidget::GetAction()
{
	return m_Action;
}
