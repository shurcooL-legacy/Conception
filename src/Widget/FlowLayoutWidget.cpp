#include "../Main.h"

FlowLayoutWidget::FlowLayoutWidget(Vector2n Position, std::initializer_list<std::shared_ptr<Widget>> Widgets, std::vector<std::shared_ptr<Behavior>> Behaviors, LayoutType Layout)
	: FlowLayoutWidget(Position, Vector2n::ZERO, Widgets, Behaviors, Layout)
{}

FlowLayoutWidget::FlowLayoutWidget(Vector2n Position, Vector2n Dimensions, std::initializer_list<std::shared_ptr<Widget>> Widgets, std::vector<std::shared_ptr<Behavior>> Behaviors, LayoutType Layout)
	: CompositeWidget(Position, Dimensions, Widgets, Behaviors),
	  m_Layout(Layout)
{
}

FlowLayoutWidget::~FlowLayoutWidget()
{
}

void FlowLayoutWidget::Render()
{
	// TODO: Refactor this out
	if (!m_Visible)
		return;

	// Layout the widgets
	if (LayoutType::Horizontal == m_Layout)
	{
		std::vector<decltype(GetWidgets().size())> VisibleWidgets;
		VisibleWidgets.reserve(GetWidgets().size());

		for (decltype(GetWidgets().size()) WidgetIndex = 0; WidgetIndex < GetWidgets().size(); ++WidgetIndex)
		{
			if (!GetWidgets()[WidgetIndex]->m_Visible)
				continue;

			VisibleWidgets.push_back(WidgetIndex);
		}

		for (decltype(VisibleWidgets.size()) WidgetIndexIndex = 0; WidgetIndexIndex < VisibleWidgets.size(); ++WidgetIndexIndex)
		{
			auto WidgetIndex = VisibleWidgets[WidgetIndexIndex];

			if (0 == WidgetIndexIndex)
			{
				GetWidgets()[WidgetIndex]->SetPosition(Vector2n(Widget::GetDimensions().X(), 0));
			}
			else
			{
				auto PrevWidgetIndex = VisibleWidgets[WidgetIndexIndex - 1];

				GetWidgets()[WidgetIndex]->SetPosition(Vector2n(GetWidgets()[PrevWidgetIndex]->GetPosition().X() + GetWidgets()[PrevWidgetIndex]->GetDimensions().X() + 2, 0));
			}
		}
	}
	else if (LayoutType::Vertical == m_Layout)
	{
		std::vector<decltype(GetWidgets().size())> VisibleWidgets;
		VisibleWidgets.reserve(GetWidgets().size());

		for (decltype(GetWidgets().size()) WidgetIndex = 0; WidgetIndex < GetWidgets().size(); ++WidgetIndex)
		{
			if (!GetWidgets()[WidgetIndex]->m_Visible)
				continue;

			VisibleWidgets.push_back(WidgetIndex);
		}

		for (decltype(VisibleWidgets.size()) WidgetIndexIndex = 0; WidgetIndexIndex < VisibleWidgets.size(); ++WidgetIndexIndex)
		{
			auto WidgetIndex = VisibleWidgets[WidgetIndexIndex];

			if (0 == WidgetIndexIndex)
			{
				GetWidgets()[WidgetIndex]->SetPosition(Vector2n(0, Widget::GetDimensions().Y()));
			}
			else
			{
				auto PrevWidgetIndex = VisibleWidgets[WidgetIndexIndex - 1];

				GetWidgets()[WidgetIndex]->SetPosition(Vector2n(0, GetWidgets()[PrevWidgetIndex]->GetPosition().Y() + GetWidgets()[PrevWidgetIndex]->GetDimensions().Y() + 2));
			}
		}
	}

	CompositeWidget::Render();
}
