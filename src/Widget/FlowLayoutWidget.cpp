#include "../Main.h"

FlowLayoutWidget::FlowLayoutWidget(Vector2n Position, std::initializer_list<std::shared_ptr<Widget>> Widgets, std::vector<std::shared_ptr<Behavior>> Behaviors)
	: CompositeWidget(Position, Widgets, Behaviors)
{
}

FlowLayoutWidget::~FlowLayoutWidget()
{
}

void FlowLayoutWidget::Render()
{
	// Layout the widgets
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
				GetWidgets()[WidgetIndex]->SetPosition(Vector2n::ZERO);
			}
			else
			{
				auto PrevWidgetIndex = VisibleWidgets[WidgetIndexIndex - 1];

				GetWidgets()[WidgetIndex]->SetPosition(Vector2n(GetWidgets()[PrevWidgetIndex]->GetPosition().X() + GetWidgets()[PrevWidgetIndex]->GetDimensions().X() + 2, 0));
			}
		}
	}

	CompositeWidget::Render();
}
