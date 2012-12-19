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
	// HACK: This doesn't belong in Render, etc.
	for (auto WidgetToLayout = 0; WidgetToLayout < GetWidgets().size(); ++WidgetToLayout)
	{
		if (0 == WidgetToLayout)
		{
			GetWidgets()[WidgetToLayout]->SetPosition(Vector2n::ZERO);
		}
		else
		{
			GetWidgets()[WidgetToLayout]->SetPosition(Vector2n(GetWidgets()[WidgetToLayout - 1]->GetPosition().X() + GetWidgets()[WidgetToLayout - 1]->GetDimensions().X() + 2, 0));
		}
	}

	CompositeWidget::Render();
}
