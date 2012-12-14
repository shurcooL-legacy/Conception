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
	// HACK: Make it work for arbitrary number of objects
	GetWidgets()[1]->SetPosition(Vector2n(GetWidgets()[0]->GetDimensions().X() + 2, 0));

	CompositeWidget::Render();
}
